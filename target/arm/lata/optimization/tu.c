#include "qemu/osdep.h"
#include "qemu/log.h"
#include "exec/tb-flush.h"
#include "target/arm/tcg/translate.h"
#include "target/arm/tcg/arm_ldst.h"
#include "accel/tcg/internal.h"
#include "lata.h"
#include "ir2.h"
#include "env.h"
#include "common.h"
#include "ir1.h"
#include "tcg/tcg.h"
#include "target/arm/lata/include/translate-a64.h"
#include "tu.h"
#include "disas/disas.h"

#ifdef CONFIG_LATA_TU
#ifdef CONFIG_SOFTMMU
#define assert_memory_lock()
#else
#define assert_memory_lock() tcg_debug_assert(have_mmap_lock())
#endif

static __thread TUControl tu_data_rel;
__thread TUControl *tu_data;
static __thread uint32_t search_buff_offset[MAX_TB_IN_TU];

static inline void tu_trees_reset(void)
{
    /* Increment the refcount first so that destroy acts as a reset */
    g_tree_ref(tu_data->tree);
    g_tree_destroy(tu_data->tree);
}

TranslationBlock *tu_tree_lookup(target_ulong pc)
{
    TranslationBlock key = {.pc = pc};
    return (TranslationBlock *)g_tree_lookup(tu_data->tree, &key);
}

static gint gpc_cmp(gconstpointer ap, gconstpointer bp)
{
    const TranslationBlock *a = (TranslationBlock *)ap;
    const TranslationBlock *b = (TranslationBlock *)bp;

    return (a->pc < b->pc) ? -1 : (a->pc > b->pc);
}

static void tu_tree_insert(TranslationBlock *tb)
{
    g_tree_insert(tu_data->tree, tb, tb);
}

static inline void tu_trees_init(void)
{
    tu_data->tree = g_tree_new(gpc_cmp);
}

void tu_control_init(void)
{
    tu_data = &tu_data_rel;
    tu_data->ir1_num_in_tu = 0;
    tu_data->tb_num = 0;
    tu_data->curr_num = 0;
    tu_data->curr_insns = 0;
    tu_trees_init();

    return;
}

static inline void tu_push_back(TranslationBlock *tb)
{
    if (!tb) {
        return;
    }
    TranslationBlock** tb_list = tu_data->tb_list;
    uint32_t* tb_num_in_tu = &tu_data->tb_num;
    tu_data->ir1_num_in_tu += tb->icount;

    tb_list[(*tb_num_in_tu)++] = tb;
}

static inline const void *qm_tb_get_code_cache(void *tb)
{
    struct TranslationBlock *ptb = (struct TranslationBlock *)tb;
    return ptb->tc.ptr;
}

int translate_tb_in_tu(struct TranslationBlock *tb)
{
    tb->tc.ptr = tcg_splitwx_to_rx(tcg_ctx->code_gen_ptr);
    tcg_ctx->gen_tb = tb;
    return tr_translate_tb(tb);
}

static inline void tu_enough_space(CPUState *cpu)
{
    if (unlikely(tcg_ctx->code_gen_ptr + MAX_TU_SIZE >= tcg_ctx->code_gen_highwater)) {
        tb_flush(cpu);
        mmap_unlock();
        /* Make the execution loop process the flush as soon as possible.  */
        cpu->exception_index = EXCP_INTERRUPT;
        cpu_loop_exit(cpu);
    }
    tcg_ctx->gen_insn_data = tcg_malloc(sizeof(uint64_t) * MAX_TB_IN_TU * TCG_MAX_INSNS * TARGET_INSN_START_WORDS);
}

static void tu_reset_tb(TranslationBlock *tb)
{
    qemu_spin_init(&tb->jmp_lock);
    tb->jmp_dest[0] = (uintptr_t)NULL;
    tb->jmp_dest[1] = (uintptr_t)NULL;
    tb->jmp_list_head = (uintptr_t)NULL;
    tb->jmp_list_next[0] = (uintptr_t)NULL;
    tb->jmp_list_next[1] = (uintptr_t)NULL;

    /* init top in and top out */
    tb->jmp_reset_offset[0] = TB_JMP_OFFSET_INVALID;
    tb->jmp_reset_offset[1] = TB_JMP_OFFSET_INVALID;
    tb_set_page_addr0(tb, -1);
    tb_set_page_addr1(tb, -1);
#ifdef CONFIG_LATA_INSTS_PATTERN
    tb->nzcv_save[0] = TB_JMP_OFFSET_INVALID;
    tb->nzcv_save[1] = TB_JMP_OFFSET_INVALID;
    tb->nzcv_use = true ;
#endif
    tb->tc.offset_in_tu = 0;
    tb->next_pc = 0;
    tb->target_pc = 0;
    tb->s_data->tu_id = 0;
    tb->s_data->is_first_tb = 0;
    tb->last_ir1_type = 0;
    tb->s_data->tu_tb_mode = TU_TB_MODE_NONE;
    tb->tu_jmp[TU_TB_INDEX_TARGET] = TB_JMP_OFFSET_INVALID;
    tb->tu_jmp[TU_TB_INDEX_NEXT] = TB_JMP_OFFSET_INVALID;
    tb->tu_unlink_stub_offset = TU_UNLINK_STUB_INVALID;

}

/* Create a TB and initialize it. */
static TranslationBlock* tb_create(CPUState *cpu, uint64_t pc,
        uint64_t cs_base, uint32_t flags, int cflags,
        int max_insns, bool is_first_tb)
{
    TranslationBlock* tb;
    tb = tcg_tb_alloc_full(tcg_ctx);
    if (unlikely(!tb)) {
        fprintf(stderr, "tb flush\n");
        /* exit(-1); */
        /* flush must be done */
        tb_flush(cpu);
        mmap_unlock();
        /* Make the execution loop process the flush as soon as possible.  */
        cpu->exception_index = EXCP_INTERRUPT;
        cpu_loop_exit(cpu);
    }        
    tu_reset_tb(tb);
    tb->pc = pc;
    tb->cs_base = cs_base;
    tb->flags = flags;
    tb->cflags = cflags;
    tb->last_ir1_type = 0;
    tb->env = cpu->env_ptr;
    target_disasm(tb, &max_insns, cpu);

    if (tb->icount == 0 || tb->s_data->tu_tb_mode == TU_TB_MODE_BROKEN ) {
        return NULL;
    }    

    tu_tree_insert(tb);
    return tb;
}

static inline int tb_sort_cmp(const void *ap, const void *bp)
{
    const TranslationBlock *a = *(const TranslationBlock **)ap;
    const TranslationBlock *b = *(const TranslationBlock **)bp;
    return a->pc - b->pc;
}

static inline uint32_t get_page(uint32_t pc)
{
    return pc & TARGET_PAGE_MASK;
}

static inline void get_tu_queue(CPUState *cpu,
		 target_ulong cs_base, uint32_t flags,
		 int cflags, int max_insns)
{
    uint64_t ir1_next_pc, ir1_target_pc;
    TranslationBlock** tb_list = tu_data->tb_list;
    uint32_t *tb_num_in_tu = &tu_data->tb_num;
    TranslationBlock *next_tb, *target_tb, *tb;
    for (int i = 0; i <  *tb_num_in_tu && *tb_num_in_tu < MAX_TB_IN_TU; i++) {
        tb = tb_list[i];
        ir1_next_pc = tb->next_pc;
        ir1_target_pc = tb->target_pc;
        switch (tb->last_ir1_type) {
        case IR1_TYPE_BRANCH:
        case IR1_TYPE_CALL:
            /* Jcc next tb should be translated without checking */
            lsassert(ir1_next_pc);
            if (get_page(tb->pc) == get_page(ir1_next_pc)) {
                next_tb = tu_tree_lookup(ir1_next_pc);
                if (!next_tb) {
                    next_tb = tb_htable_lookup(cpu,
                        ir1_next_pc, cs_base, flags, cflags);
                }
                if (!next_tb) {
                    next_tb = tb_create(cpu, ir1_next_pc, cs_base, flags,
                                   cflags, max_insns, 0);
                    tu_push_back(next_tb);
                }
                if (next_tb && next_tb->tc.ptr != NULL) {
                    tb->tu_jmp[TU_TB_INDEX_NEXT] = 0;
                }
                tb->next_tb[TU_TB_INDEX_NEXT] = next_tb;
            } else {
                tb->next_tb[TU_TB_INDEX_NEXT] = NULL;
            }
            if (tb->tu_jmp[TU_TB_INDEX_NEXT] == 0) {
                lsassert(tb->next_tb[TU_TB_INDEX_NEXT]);
            }

            lsassert(ir1_target_pc);
            /* Jcc target tb should not be translated if the distance is too far */
            if (get_page(tb->pc) == get_page(ir1_target_pc)) {
                target_tb = tu_tree_lookup(ir1_target_pc);
                if (!target_tb) {
                    target_tb = tb_htable_lookup(cpu,
                        ir1_target_pc, cs_base, flags, cflags);
                }
                if (!target_tb) {
                    target_tb = tb_create(cpu, ir1_target_pc, cs_base, flags,
                                     cflags, max_insns, 0);
                    tu_push_back(target_tb);
                }
                if (target_tb && target_tb->tc.ptr != NULL) {
                    tb->tu_jmp[TU_TB_INDEX_TARGET] = 0;
                }
                tb->next_tb[TU_TB_INDEX_TARGET] = target_tb;
            } else {
                tb->next_tb[TU_TB_INDEX_TARGET] = NULL;
            }

            if (tb->tu_jmp[TU_TB_INDEX_TARGET] == 0) {
                lsassert(tb->next_tb[TU_TB_INDEX_TARGET]);
            }
            break;
        case IR1_TYPE_JMP:
            /* JMP target tb should not be translated if the distance is too far */
            if (get_page(tb->pc) == get_page(ir1_target_pc)) {
                target_tb = tu_tree_lookup(ir1_target_pc);
                if (!target_tb) {
                    target_tb = tb_htable_lookup(cpu,
                        ir1_target_pc, cs_base, flags, cflags);
                }
                if (get_page(tb->pc) == get_page(ir1_target_pc) && !target_tb) {
                    target_tb = tb_create(cpu, ir1_target_pc, cs_base, flags,
                                     cflags, max_insns, 0);
                    tu_push_back(target_tb);
                }
                tb->next_tb[TU_TB_INDEX_TARGET] = target_tb;
            } else {
                tb->next_tb[TU_TB_INDEX_TARGET] = NULL;
            }
            break;
        case IR1_TYPE_CALLIN:
        case IR1_TYPE_NORMAL:
        case IR1_TYPE_SYSCALL:
            lsassert(ir1_next_pc);
            if (get_page(tb->pc) == get_page(ir1_next_pc)) {
                next_tb = tu_tree_lookup(ir1_next_pc);
                if (!next_tb) {
                    next_tb = tb_htable_lookup(cpu,
                        ir1_next_pc, cs_base, flags, cflags);
                }
                if (!next_tb) {
                    next_tb = tb_create(cpu, ir1_next_pc, cs_base, flags,
                                   cflags, max_insns, 0);
                    tu_push_back(next_tb);
                }
                tb->next_tb[TU_TB_INDEX_NEXT] = next_tb;
            } else {
                tb->next_tb[TU_TB_INDEX_NEXT] = NULL;
            }
            break;
        case IR1_TYPE_RET:
        case IR1_TYPE_JMPIN:
            break;
        default:
            lsassert(0);
        }
    }

/* Note: *tb_num_in_tu maybe greater than MAX_TB_IN_TU. */
    if (*tb_num_in_tu > MAX_TB_IN_TU + 1) {
        lsassert(0);
    }
}

static void tu_set_tb_to_translate_context(TranslationBlock *tb)
{
    tb->next_tb[TU_TB_INDEX_NEXT] = NULL;
    tb->next_tb[TU_TB_INDEX_TARGET] = NULL;
    tb->s_data->tu_tb_mode = TU_TB_MODE_SWITCH_TO_TB;
    tb->tu_jmp[TU_TB_INDEX_TARGET] = TB_JMP_OFFSET_INVALID;
    tb->tu_jmp[TU_TB_INDEX_NEXT] = TB_JMP_OFFSET_INVALID;
}

static void tu_split_tb(TranslationBlock *pre_tb, TranslationBlock *tb)
{
    bool spilt_success = false;
    DisasContext *pre_s = (DisasContext *)pre_tb->ir1;
    uint32_t pre_pc, tb_pc = ((DisasContext *)tb->ir1)->pc_curr;
    if (pre_tb->icount < tb->icount) {
        for (int i = 0; i < pre_tb->icount; i++) {
            pre_pc = (pre_s + i)->pc_curr;
            if (pre_pc == tb_pc) {
                spilt_success = true;
                break;
            }
        }
    } else if (pre_tb->icount > tb->icount) {
        uint32_t th_firs_inst = pre_tb->icount - tb->icount;
        lsassert(th_firs_inst <= pre_tb->icount);
        pre_pc = (pre_s + th_firs_inst)->pc_curr;
        if (pre_pc == tb_pc) {
            spilt_success = true;
        }
    } else {
        lsassert(0);
    }

    if (spilt_success) {
        /* tb->tu_start_mode = TU_TB_START_NORMAL; */
        pre_tb->next_tb[TU_TB_INDEX_NEXT] = tb;
        pre_tb->next_tb[TU_TB_INDEX_TARGET] = NULL;
        pre_tb->size -= tb->size;
        pre_tb->icount -= tb->icount;
        pre_tb->next_pc = tb->pc;
        pre_tb->tu_jmp[TU_TB_INDEX_TARGET] = TB_JMP_OFFSET_INVALID;
        pre_tb->tu_jmp[TU_TB_INDEX_NEXT] = TB_JMP_OFFSET_INVALID;
        pre_tb->isplit = 1;
    } else {
        /*Can't judge pre_tb or tb was broken.*/
        tu_set_tb_to_translate_context(pre_tb);
        tu_set_tb_to_translate_context(tb);
    }
}

void solve_tb_overlap(uint tb_num_in_tu,
		TranslationBlock **tb_list, int max_insns)
{
    TranslationBlock *pre_tb = tb_list[0];
    TranslationBlock *tb;
    for (int i = 1; i < tb_num_in_tu; i++) {
        tb = tb_list[i];
        tb->isplit = 0;
        if (pre_tb->s_data->tu_tb_mode == TU_TB_MODE_BROKEN) {
            assert(0);
            pre_tb = tb;
            continue;
        }
        /* judge if TB need split
         * pre_tb-------->pc + 0
         *                pc + 1
         *                pc + 2
         * tb------------>pc + 3
         *                pc + 4
         *                pc + 5
         * pre_tb_next--->pc + 6
         */
        if (tb->pc < pre_tb->next_pc && tb->pc > pre_tb->pc) {
            /* when split prefix ins, translate it two times.
             *
             * pre_tb--------> lock
             * tb------------> add
             *                 ...
             * tb_end--------> jmp
             *
             */
            if (pre_tb->icount == tb->icount) {
                /* pre_tb dont link in TU */
                pre_tb->next_tb[TU_TB_INDEX_NEXT] = NULL;
                pre_tb->next_tb[TU_TB_INDEX_TARGET] = NULL;
                pre_tb->tu_jmp[TU_TB_INDEX_TARGET] =
                    TB_JMP_OFFSET_INVALID;
                pre_tb->tu_jmp[TU_TB_INDEX_NEXT] =
                    TB_JMP_OFFSET_INVALID;
                pre_tb->s_data->tu_tb_mode = TU_TB_MODE_SWITCH_TO_TB;
                pre_tb = tb;
                continue;
            }
            if (pre_tb->icount < tb->icount) {
                /*Can't judge pre_tb or tb was broken.*/
                tu_set_tb_to_translate_context(pre_tb);
                tu_set_tb_to_translate_context(tb);
                pre_tb = tb;
                continue;
            }
            /*if pre_tb is splited, unlink pre_tb in tu */
            pre_tb->next_tb[TU_TB_INDEX_NEXT] = NULL;
            pre_tb->next_tb[TU_TB_INDEX_TARGET] = NULL;

            /*When pre_tb->icount achive max_insns, the end addresses of two TBS are different*/
            if (pre_tb->icount == max_insns) {
                pre_tb->tu_jmp[TU_TB_INDEX_TARGET] =
                    TB_JMP_OFFSET_INVALID;
                pre_tb->tu_jmp[TU_TB_INDEX_NEXT] =
                    TB_JMP_OFFSET_INVALID;
            } else {
                tu_split_tb(pre_tb, tb);
            }
            /* Note: some message of pre_tb last ir1 ins is not modify */
        }
        if (pre_tb->s_data->tu_tb_mode != TU_TB_MODE_BROKEN) {
            pre_tb = tb;
        }
    }
}

/*explore tbs in tu*/
static TranslationBlock *tb_explore(CPUState *cpu,
                              target_ulong pc, uint64_t cs_base,
                              uint32_t flags, int cflags)
{
    TranslationBlock** tb_list = tu_data->tb_list;
    uint32_t *tb_num_in_tu = &tu_data->tb_num;
    uint32_t *ir1_num_in_tu = &tu_data->ir1_num_in_tu;
    int max_insns;

    max_insns = cflags & CF_COUNT_MASK;
    if (max_insns == 0) {
        max_insns = TCG_MAX_INSNS;
    }
    if (max_insns > TCG_MAX_INSNS) {
        max_insns = TCG_MAX_INSNS;
    }
    if (cpu->singlestep_enabled) {
        max_insns = 1;
    }
    *tb_num_in_tu = 0;
    *ir1_num_in_tu = 0;

    tu_enough_space(cpu);
    /* the entry used as return value*/

    TranslationBlock *entry = tb_create(cpu, pc, cs_base,
            flags, cflags, max_insns, true);
    tu_push_back(entry);            
    /* search all tbs we can get */
    get_tu_queue(cpu, cs_base, flags, cflags, max_insns);

    /* sort tbs by PC */
    qsort(tb_list, *tb_num_in_tu, sizeof(TranslationBlock *), tb_sort_cmp);

    /* Some TBS may overlap. We split these overlapping TBS. */
    solve_tb_overlap(*tb_num_in_tu, tb_list, max_insns);

    for (int i = 0; i < *tb_num_in_tu; i++){
        /* record tu pc and tb_num */
        tb_list[i]->s_data->tu_id = tb_list[0]->pc;
    }

    return entry;

}

/*fix address if b jmp target has been translated*/
void tu_relocat_branch(TranslationBlock * tb, int n)
{
    TranslationBlock *tb_next = tb->next_tb[n];
    uintptr_t fix_addr = (uintptr_t)tb->tc.ptr + tb->jmp_insn_offset[n];
    uint fix_insn = *(uint *)fix_addr;

    if ((fix_insn & 0xFC000000) == 0x50000000) {/*b*/
        tb_set_jmp_target(tb, n, (uintptr_t)tb_next->tc.ptr);
    }
}

/* translate tu from ir1->ir2 and link tbs if next_tb in tu*/
void translate_tu(uint32 tb_num_in_tu, TranslationBlock **tb_list)
{
    if (tb_num_in_tu == 0) {
        return;
    }
    int gen_code_size;
    uint32_t search_size = 0;
    TranslationBlock *tb;

    qatomic_set(&tcg_ctx->code_gen_ptr, (void *)
            ROUND_UP((uintptr_t)tcg_ctx->code_gen_ptr, CODE_GEN_ALIGN));

    for (uint32_t i = 0; i < tb_num_in_tu; i++) {
        tu_data->curr_num = i;
        tb = tb_list[i];
        tb->tu_unlink_stub_offset = TU_UNLINK_STUB_INVALID;
        gen_code_size = translate_tb_in_tu(tb);
        lata_fast_jmp_cache_add(tb->env, tb->pc, (uint64_t)(tb->tc.ptr));
        tu_data->curr_insns = tb->icount;
        qatomic_set(&tcg_ctx->code_gen_ptr, (void *)ROUND_UP((uintptr_t)tcg_ctx->code_gen_ptr + gen_code_size, CODE_GEN_ALIGN));
        tb->tc.size = gen_code_size;
        tb->tc.offset_in_tu = tb->tc.ptr - tb_list[0]->tc.ptr;
        /* init original jump addresses which have been set during tcg_gen_code() */
        if (tb->jmp_reset_offset[0] != TB_JMP_OFFSET_INVALID) {
            tb_reset_jump(tb, 0);
        }
        if (tb->jmp_reset_offset[1] != TB_JMP_OFFSET_INVALID) {
            tb_reset_jump(tb, 1);
        }

        search_buff_offset[i] = search_size;
        search_size += encode_search(tb, (void *)(tcg_ctx->tb_gen_tail) + search_buff_offset[i]);
    }

    tb_list[0]->s_data->tu_size = tcg_ctx->code_gen_ptr - tb_list[0]->tc.ptr;

    for (int i = 0; i < tb_num_in_tu; i++) {
        tb = tb_list[i];
        if (tb->last_ir1_type == IR1_TYPE_JMPIN || tb->last_ir1_type == IR1_TYPE_CALLIN || tb->last_ir1_type == IR1_TYPE_SYSCALL) {
            continue;
        }
        if (tb->next_tb[TU_TB_INDEX_NEXT] != NULL && tb->last_ir1_type != IR1_TYPE_CALL) {
            tu_relocat_branch(tb, TU_TB_INDEX_NEXT);
        }        
        if ((tb->next_tb[TU_TB_INDEX_TARGET] != NULL)) {
            tu_relocat_branch(tb, TU_TB_INDEX_TARGET);
            tb->tu_link_ins = *(uint32_t *)(tb->tc.ptr +
                    tb->tu_jmp[TU_TB_INDEX_TARGET]);
        }
    }

    /*search data*/
    for (int i = 0; i < tb_num_in_tu; i++) {
        tb = tb_list[i];
        tb->tu_search_addr = (uint8_t *)
            ((void *)(tcg_ctx->tb_gen_tail) + search_buff_offset[i]);
    }
    tb_list[0]->s_data->tu_size = tcg_ctx->code_gen_ptr - tb_list[0]->tc.ptr;

}

/* print tu if open in_asm/out_asm debug mode */
static void print_tu(CPUState *cpu, uint32 tb_num_in_tu, TranslationBlock **tb_list){
    if (qemu_loglevel_mask(CPU_LOG_TB_IN_ASM)
        && qemu_log_in_addr_range(tb_list[0]->pc)) {
        FILE *logfile = qemu_log_trylock();
        fprintf(logfile, "TU IN: %s\n", lookup_symbol(tb_list[0]->pc));
        fprintf(logfile, "-----------------------------------------\n");        
        for (int i = 0; i < tb_num_in_tu; i++) {
            target_disas(logfile, cpu, tb_list[i]->pc, tb_list[i]->size);
            fprintf(logfile, "-----------------------------------------\n");            
        }
        qemu_log_unlock(logfile);
    }

    if (qemu_loglevel_mask(CPU_LOG_TB_OUT_ASM) &&
        qemu_log_in_addr_range(tb_list[0]->pc)) {
        FILE *logfile = qemu_log_trylock();
        if (logfile) {
            int code_size = tb_list[0]->s_data->tu_size;
            // size_t chunk_start;
            // int insn = 0;

            /* Dump header and the first instruction */
            fprintf(logfile, "\n");
            fprintf(logfile, "TU OUT: [size=%d]\n", code_size);
            for (int i = 0; i < tb_num_in_tu; i++) {
                fprintf(logfile, "  -- tb%d: guest addr 0x%016" PRIx64 " \n",
                    i, (uint64_t)tb_list[i]->pc); 
                int gen_code_size = tb_list[i]->codesize;
                disas(logfile, tb_list[i]->tc.ptr, gen_code_size);                    

            }

            fprintf(logfile, "\n");
            qemu_log_unlock(logfile);
        }
    }
}

static void register_tu(uint32 tb_num_in_tu, TranslationBlock **tb_list,
		CPUState *cpu, int cflags)
{
    for (int i = 0; i < tb_num_in_tu; i++) {
        TranslationBlock *tb = tb_list[i];
        if (tb->s_data->tu_tb_mode != TU_TB_MODE_BROKEN) {
            CPUArchState *env = cpu->env_ptr;
            tb_page_addr_t phys_pc;
            // tb_page_addr_t phys_page2;
            // target_ulong virt_page2;
            phys_pc = get_page_addr_code(env, tb->pc);
            if (phys_pc == -1) {
                /* Generate a one-shot TB with 1 insn in it */
                cflags = (cflags & ~CF_COUNT_MASK) | CF_LAST_IO | 1;
            }
            tb_set_page_addr0(tb, phys_pc);
            tb_set_page_addr1(tb, -1);
            if (phys_pc != -1) {
                tb_lock_page0(phys_pc);
            }
            // phys_page2 = -1;
            /* should inset TB to Qht */
            tb_link_page(tb);
            tcg_tb_insert(tb);
        }
    }

}

/* Called with mmap_lock held for user mode emulation.  */
TranslationBlock *tu_gen_code(CPUState *cpu,
                              target_ulong pc, uint64_t cs_base,
                              uint32_t flags, int cflags)
{
    TranslationBlock* entry;
    TranslationBlock** tb_list = tu_data->tb_list;
    uint32_t *tb_num_in_tu = &tu_data->tb_num;

    assert_memory_lock();
    qemu_thread_jit_write();

    /* 1.Search all TBs in the function */
    entry = tb_explore(cpu, pc, cs_base, flags, cflags);
    /* 2.Translate TU*/
    translate_tu(*tb_num_in_tu, tb_list);
    /* 3.Insert TBs in TU to QHT */
    register_tu(*tb_num_in_tu, tb_list, cpu, cflags);
    print_tu(cpu, *tb_num_in_tu, tb_list);
    tu_trees_reset();
    return entry;
}
#endif