#include "qemu/osdep.h"
#include "qemu/log.h"
#include "target/arm/tcg/translate.h"
#include "target/arm/tcg/arm_ldst.h"
#include "ir2.h"
#include "env.h"
#include "common.h"
#include "tu.h"
#include "tcg/tcg.h"

#ifdef CONFIG_LATA_TU
#ifdef CONFIG_SOFTMMU
#define assert_memory_lock()
#else
#define assert_memory_lock() tcg_debug_assert(have_mmap_lock())
#endif

static __thread TUControl tu_data_rel;
__thread TUControl *tu_data;
static __thread TranslationBlock tu_tb_tmp;
static __thread uint8_t search_buff[MAX_TB_IN_TU * TCG_MAX_INSNS];
static __thread uint32_t search_buff_offset[MAX_TB_IN_TU];
static DisasContext ir1_list_rel[TCG_MAX_INSNS * MAX_TB_IN_CACHE];

static inline void tu_trees_reset(void)
{
        /* Increment the refcount first so that destroy acts as a reset */
        g_tree_ref(tu_data->tree);
        g_tree_destroy(tu_data->tree);
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
    tu_trees_init();

    return;
}

static inline void tb_push_back(TranslationBlock *tb)
{
    if (!tb) {
        return;
    }
    TranslationBlock** tb_list = tu_data->tb_list;
    uint32_t* tb_num_in_tu = &tu_data->tb_num;

    tb_list[(*tb_num_in_tu)++] = tb;
}

static inline const void *qm_tb_get_code_cache(void *tb)
{
    struct TranslationBlock *ptb = (struct TranslationBlock *)tb;
    return ptb->tc.ptr;
}

static int tr_translate_tb(struct TranslationBlock *tb)
{
    fprintf(stderr, "[LATX] start translation.\n");

    tr_init(tb);
    bool translation_done = tr_ir2_generate(tb);
    int gen_code_size = 0;
    if(translation_done){
        gen_code_size = tr_ir2_assemble(qm_tb_get_code_cache(tb)) * 4;
    }
    
    tr_fini();

    tcg_ctx->gen_insn_data =
        tcg_malloc(sizeof(uint64_t) * tcg_ctx->gen_tb->icount * tcg_ctx->insn_start_words);
    return gen_code_size;
}

int translate_tb_in_tu(struct TranslationBlock *tb)
{
    tb->tc.ptr = tcg_splitwx_to_rx(tcg_ctx->code_gen_ptr);
    if (tb->tu_tb_mode == TU_TB_MODE_BROKEN) {
        return tu_switch_native_tb(tb);
    } else {
        return tr_translate_tb(tb);
    }
}

void lata_pre_translate(void** list, int num, CPUState *cpu,
                        uint64_t cs_base, uint32_t flags, uint32_t cflags)
{
/*     TranslationBlock *next = NULL;
    TranslationBlock *curr = NULL;

    while (num--) {
        curr = *(TranslationBlock**)list;
        while (curr->next_86_pc) {
            next = tb_lookup(cpu, curr->next_86_pc, cs_base, flags, cflags);
            if (next == NULL) {
                next = tb_gen_code(cpu, curr->next_86_pc, cs_base, flags, cflags);
            }
            // fix self-modification
            int index = 0;
            if (curr->page_addr[1] != -1) {
                index = 1;
            }
            if (option_jr_ra && curr->return_target_ptr
                && (curr->page_addr[index] == next->page_addr[0])) {
                int temp0 = reg_itemp_map[ITEMP0];
                int temp1 = reg_itemp_map[ITEMP1];
                uint64_t patch_pcalau12i, patch_ori;
                uint64_t patch_scr1, patch_scr0;

                ptrdiff_t offset_high = (((uint64_t)next->tc.ptr >> 12) -
                                        ((uintptr_t)curr->return_target_ptr >> 12))
                                        & 0xfffff;
                ptrdiff_t offset_low = (uint64_t)next->tc.ptr & 0xfff;

                // pcalau12i itemp0, offset_high
                patch_pcalau12i = 0x1a000000 | temp0 | (offset_high << 5);
                // ori itemp0, itemp0, offset_low
                patch_ori = 0x03800000 | (offset_low << 10) |
                            (temp0 << 5) | temp0;
                // gr2scr scr1, itemp0
                patch_scr1 = 0x801 | (temp0 << 5);
                // gr2scr scr0, itemp1
                patch_scr0 = 0x800 | (temp1 << 5);
                *curr->return_target_ptr = (patch_pcalau12i | (patch_ori << 32));
                *(curr->return_target_ptr + 1) = (patch_scr1 | (patch_scr0 << 32));
            }
            curr = next;
        }
        list++;
    } */
    printf("here!\n");
}

static DisasContext *translate_aarch64_insn(CPUState *cpu, DisasContext *s)
{
    CPUARMState *env = cpu->env_ptr;
    uint64_t pc = s->base.pc_next;
    uint32_t insn;

    s->pc_curr = pc;
    insn = arm_ldl_code(env, &s->base, pc, s->sctlr_b);
    s->insn = insn;
    s->base.pc_next = pc + 4;

    s->fp_access_checked = false;
    s->sve_access_checked = false;

    if (dc_isar_feature(aa64_bti, s)) {
        if (s->base.num_insns == 1) {

            s->guarded_page = is_guarded_page(env, s);

            /* First insn can have btype set to non-zero.  */
            tcg_debug_assert(s->btype >= 0);

            if (s->btype != 0
                && s->guarded_page
                && !btype_destination_ok(insn, s->bt, s->btype)) {
                gen_exception_insn(s, 0, EXCP_UDEF, syn_btitrap(s->btype));
                return;
            }
        } else {
            /* Not the first insn: btype must be 0.  */
            tcg_debug_assert(s->btype == 0);
        }
    }

    s->is_nonstreaming = false;

    //don't support sme and sve
    if (!disas_a64(s, insn) ) {
        disas_a64_legacy(s, insn);
    }

    if (s->btype > 0 && s->base.is_jmp != DISAS_NORETURN) {
        reset_btype(s);
    }

    return s;
}

DisasContext *get_ir1_list(CPUState *cpu, struct TranslationBlock *tb, uint64_t pc, int max_insns)
{
 /* get aarch64 instructions */
    // tb->aarch64_insns = get_ir1_list(tb, tb->pc, max_insns);
    static uint8_t inst_cache[TCG_MAX_INSNS];
    DisasContext *pir1;
    uint64_t start_pc = pc;

    uint32_t *ir1_num_in_tu = &(tu_data->ir1_num_in_tu);
    DisasContext *ir1_list = ir1_list_rel + (*ir1_num_in_tu);

    DisasContextBase *db;
    db->tb = tb;
    db->pc_first = pc;
    db->pc_next = pc;
    db->is_jmp = DISAS_NEXT;
    db->num_insns = 0;
    db->max_insns = max_insns;

    while (true) {
        max_insns = ++db->num_insns;
        // disasemble this instruction 
        pir1 = &ir1_list[max_insns];
        pir1->base = *db;
        // get next pc 
        translate_aarch64_insn(cpu, pir1);

        /* Stop translation if translate_insn so indicated.  */
        if (db->is_jmp != DISAS_NEXT) {
            break;
        }

        if (db->num_insns >= db->max_insns) {
            db->is_jmp = DISAS_TOO_MANY;
            break;
        }
    }
    
    tb->size =db->pc_next - db->pc_first;
    tb->icount = db->num_insns;

    *ir1_num_in_tu += max_insns;

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
    // tb->_top_out = -1;
    // tb->_top_in = -1;
    tb->jmp_reset_offset[0] = TB_JMP_OFFSET_INVALID;
    tb->jmp_reset_offset[1] = TB_JMP_OFFSET_INVALID;
    tb->jmp_insn_offset[0] = TB_JMP_OFFSET_INVALID;
    tb->jmp_insn_offset[1] = TB_JMP_OFFSET_INVALID;
    // tb->page_addr[0] = tb->page_addr[1] = -1;
    // tb->next_aarch64_pc = 0;
    // tb->return_target_ptr = NULL;
#ifdef CONFIG_LATX_TU
    // tb->tu_start_mode = TU_TB_START_NONE;
    tb->next_pc = 0;
    tb->target_pc = 0;
    tb->tu_aarch64_pc = 0;
    tb->is_first_tb = 0;
    tb->last_aarch64_type = 0;
    tb->tb_num = 0;
    tb->tu_tb_mode = TU_TB_MODE_NONE;
    tb->tu_jmp[0] = TB_JMP_OFFSET_INVALID;
    tb->tu_jmp[1] = TB_JMP_OFFSET_INVALID;
    // tb->jmp_target_arg[0] = TB_JMP_OFFSET_INVALID;
    // tb->jmp_target_arg[1] = TB_JMP_OFFSET_INVALID;
    // tb->jmp_stub_reset_offset[0] = TB_JMP_OFFSET_INVALID;
    // tb->jmp_stub_reset_offset[1] = TB_JMP_OFFSET_INVALID;
#endif

    tb->next_tb[0] = NULL;
    tb->next_tb[1] = NULL;
}


/* Create a TB and initialize it. */
static TranslationBlock* tb_create(CPUState *cpu, uint64_t pc,
        uint64_t cs_base, uint32_t flags, int cflags,
        int max_insns, bool is_first_tb)
{
    CPUArchState *env = cpu->env_ptr;
    TranslationBlock* tb;
    tb_page_addr_t phys_pc,phys_page2;
    uint64_t virt_page2;
    void *host_pc;

    if (!(cflags & CF_PCREL)) {
        tb->pc = pc;
    }
    tu_reset_tb(tb);
    tb->cs_base = cs_base;
    tb->flags = flags;
    tb->cflags = cflags;
/*     // tb->trace_vcpu_dstate = *cpu->trace_dstate;
    // tcg_ctx->tb_cflags = cflags;
    // tb->_top_out = -1;
    // tb->_top_in = -1;
    // tb->tc.ptr = NULL;
    // tb->tc.size = 0;
    // tb->next_aarch64_pc = 0;
    // tb->return_target_ptr = NULL;
    // tb->is_first_tb = 0;
    // tb->last_aarch64_type = 0;
    // tb->need_fix = 0;
    // tb->tb_num = 0; */

    //TODO
    target_disasm(tb, max_insns, cpu);
    // gen_code_size = setjmp_gen_code(env, tb, pc, host_pc, &max_insns, &ti);

    if (tb->icount == 0 || tb->tu_tb_mode == TU_TB_MODE_BROKEN) {
        return NULL;
    }    
    tb = tcg_tb_alloc(tcg_ctx);
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

    memcpy(tb, &tu_tb_tmp, sizeof(TranslationBlock));
    tu_tree_insert(tb);
    return tb;
}

static inline int tb_sort_cmp(const void *ap, const void *bp)
{
    const TranslationBlock *a = *(const TranslationBlock **)ap;
    const TranslationBlock *b = *(const TranslationBlock **)bp;
    return a->pc - b->pc;
}

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

    tu_enough_space(cpu);
    /* the entry used as return value*/
    *tb_num_in_tu = 0;
    *ir1_num_in_tu = 0;
    TranslationBlock *entry = tb_create(cpu, pc, cs_base,
            flags, cflags, max_insns, true);
    /* init bitmap */
    TranslationBlock *next_tb, *target_tb, *tb = entry;
    tb_push_back(tb);
    ADDRX aarch64_next_pc, aarch64_target_pc;

    /* search all tbs we can get */
    for (int i = 0; i <  *tb_num_in_tu; i++) {
        tb = tb_list[i];
        aarch64_next_pc = tb->next_pc;
        aarch64_target_pc = tb->target_pc;

        switch (tb->last_aarch64_type) {
        case AARCH64_TYPE_BRANCH:

/*          //Jcc next tb should be translated without checking 
            next_tb = tb_htable_lookup(cpu, ir1_next_pc, cs_base, flags, cflags);
            if (next_tb && next_tb->tc.ptr != NULL) {tb->need_fix |= 0x1;}
            if (!next_tb) {
                next_tb = tb_create(cpu, ir1_next_pc, cs_base, flags,
                                   cflags, max_insns, 0);
                tb_push_back(next_tb);
            }
            tb->next_tb[0] = next_tb;

            //Jcc target tb should not be translated if the distance is too far
            target_tb = tb_htable_lookup(cpu, ir1_target_pc, cs_base, flags, cflags);

            if (target_tb && target_tb->tc.ptr != NULL) {tb->need_fix |= 0x2;}
            if (get_page(tb->pc) == get_page(ir1_target_pc) && !target_tb) {
                target_tb = tb_create(cpu, ir1_target_pc, cs_base, flags,
                                     cflags, max_insns, 0);
                tb_push_back(target_tb);
            }
            tb->next_tb[1] = target_tb;
            break;  */

        case AARCH64_TYPE_JUMP:
            /* JMP target tb should not be translated if the distance is too far */
/*             target_tb = tb_htable_lookup(cpu, ir1_target_pc, cs_base, flags, cflags);
            if (get_page(tb->pc) == get_page(ir1_target_pc) && !target_tb) {
                target_tb = tb_create(cpu, ir1_target_pc, cs_base, flags,
                                     cflags, max_insns, 0);
                tb_push_back(target_tb);
            }
            tb->next_tb[1] = target_tb;
            break; */
        case AARCH64_TYPE_CALL:
        case AARCH64_TYPE_NORMAL:
        case AARCH64_TYPE_SYSCALL:
/*             next_tb = tb_htable_lookup(cpu, ir1_next_pc, cs_base, flags, cflags);
            if (!next_tb) {
                next_tb = tb_create(cpu, ir1_next_pc, cs_base, flags,
                                   cflags, max_insns, 0);
                tb_push_back(next_tb);
            }
            tb->next_tb[0] = next_tb;
            break; */
        case AARCH64_TYPE_JUMPIN:
        case AARCH64_TYPE_RET:
/*             break; */
        default:
            assert(0);
        }
    }
    /* sort tbs by PC */
    qsort(tb_list, *tb_num_in_tu, sizeof(TranslationBlock *), tb_sort_cmp);

    /* Some TBS may overlap. We split these overlapping TBS. */
    TranslationBlock *pre_tb = tb_list[0];
    for (int i = 1; i < *tb_num_in_tu; i++) {
        tb = tb_list[i];
        if (pre_tb->tu_tb_mode == TU_TB_MODE_BROKEN) {
            pre_tb = tb;
            continue;
        }
        /* judge if TB need 
        split
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
                pre_tb->next_tb[0] = NULL;
                pre_tb->next_tb[1] = NULL;
                pre_tb->tu_jmp[0] = TB_JMP_OFFSET_INVALID;
                pre_tb->tu_jmp[1] = TB_JMP_OFFSET_INVALID;
                pre_tb->tu_tb_mode = TU_TB_MODE_SWITCH_TO_TB;
                continue;
            }
            if (pre_tb->icount < tb->icount) {
                /* this situation shouldn't happen after sort*/
                assert(0);
            }
            /*if pre_tb is splited, unlink pre_tb in tu */
            pre_tb->next_tb[0] = NULL;
            pre_tb->next_tb[1] = NULL;
            /*When pre_tb->icount achive max_insns, the end addresses of two TBS are different*/
            if (pre_tb->icount == max_insns) {
                pre_tb->tu_jmp[0] =TB_JMP_OFFSET_INVALID;
                pre_tb->tu_jmp[1] = TB_JMP_OFFSET_INVALID;
            } else {
                tu_split_tb(pre_tb, tb);
            }
            /* Note: some message of pre_tb last aarch64 ins is not modify */
        }
        if (pre_tb->tu_tb_mode != TU_TB_MODE_BROKEN) {
            pre_tb = tb;
        }
    }
    /* tb_list[*tb_num_in_tu - 1]->need_fix = 0; */

    for (int i = 0; i < *tb_num_in_tu; i++){
        /* record tu pc and tb_num */
        tb_list[i]->tu_aarch64_pc = tb_list[0]->pc;
        tb_list[i]->tb_num = *tb_num_in_tu;
    }

    tb_list[0]->is_first_tb = 1;
    return entry;
}

/* Called with mmap_lock held for user mode emulation.  */
TranslationBlock *tu_gen_code(CPUState *cpu,
                              target_ulong pc, uint64_t cs_base,
                              uint32_t flags, int cflags)
{
    TranslationBlock* entry, *tb;
    TranslationBlock** tb_list = tu_data->tb_list;
    uint32_t *tb_num_in_tu = &tu_data->tb_num;
    int gen_code_size;
    int tu_size = 0;
    uint32_t search_size = 0;

    assert_memory_lock();
    qemu_thread_jit_write();
    /* 1.Search all TBs in the function */
    entry = tb_explore(cpu, pc, cs_base, flags, cflags);

    // tcg_insn_unit *gen_code_buf;
    // gen_code_buf = tcg_ctx->code_gen_ptr;

    for (uint32_t i = 0; i < *tb_num_in_tu; i++) {
        tb = tb_list[i];

        gen_code_size = translate_tb_in_tu(tb);
        tb->tc.size = gen_code_size;
        tb->tc.offset_in_tu = tu_size;
        tu_size += gen_code_size;
        uintptr_t gen_code_buf = (uintptr_t)tcg_ctx->code_gen_ptr + gen_code_size;
        qatomic_set(&tcg_ctx->code_gen_ptr, (void *)gen_code_buf);
        /* fprintf(stderr, "i %d, tcg_ctx->code_gen_ptr %p \n", i, (tcg_ctx->code_gen_ptr)); */
        /* fprintf(stderr, "i %d, gen_code_buf %x \n", i, *gen_code_buf); */
        /* init jump list */
        qemu_spin_init(&tb->jmp_lock);
        tb->jmp_list_head = (uintptr_t)NULL;
        tb->jmp_list_next[0] = (uintptr_t)NULL;
        tb->jmp_list_next[1] = (uintptr_t)NULL;
        tb->jmp_dest[0] = (uintptr_t)NULL;
        tb->jmp_dest[1] = (uintptr_t)NULL;
        /* init original jump addresses which have been set during tcg_gen_code() */
        if (tb->jmp_reset_offset[0] != TB_JMP_OFFSET_INVALID
            && tb->tu_jmp[0] == TB_JMP_OFFSET_INVALID ){
            tb_reset_jump(tb, 0);
        }
        if (tb->jmp_reset_offset[1] != TB_JMP_OFFSET_INVALID
            && tb->tu_jmp[1] == TB_JMP_OFFSET_INVALID ) {
            tb_reset_jump(tb, 1);
        }
        search_buff_offset[i] = search_size;
        search_size += encode_search(tb, search_buff + search_buff_offset[i]);

    }
    tb_list[0]->tu_size = tu_size;
    for (int i = 0; i < *tb_num_in_tu; i++) {
        tb = tb_list[i];
        if (tb->tu_jmp[0] != TB_JMP_OFFSET_INVALID) {
            tu_relocat_target_branch(tb);
        }
        if (tb->tu_jmp[1] != TB_JMP_OFFSET_INVALID) {
            tu_relocat_next_branch(tb);
        }
    }

    memcpy(tcg_ctx->code_gen_ptr, search_buff, search_size);
    qatomic_set(&tcg_ctx->code_gen_ptr, (void *)
            ROUND_UP((uintptr_t)tcg_ctx->code_gen_ptr + search_size,
                CODE_GEN_ALIGN));
    for (int i = 0; i < *tb_num_in_tu; i++) {
        tb = tb_list[i];
        if (tb->tu_tb_mode != TU_TB_MODE_BROKEN) {
            CPUArchState *env = cpu->env_ptr;
            tb_page_addr_t phys_pc;
            tb_page_addr_t phys_page2;
            target_ulong virt_page2;
            phys_pc = get_page_addr_code(env, tb->pc);
            if (phys_pc == -1) {
                /* Generate a one-shot TB with 1 insn in it */
                cflags = (cflags & ~CF_COUNT_MASK) | CF_LAST_IO | 1;
            }
            /* should inset TB to Qht */
            virt_page2 = (tb->pc + tb->size - 1) & TARGET_PAGE_MASK;
            phys_page2 = -1;
            if ((tb->pc & TARGET_PAGE_MASK) != virt_page2) {
                phys_page2 = get_page_addr_code(env, virt_page2);
            }
            tb_link_page(tb, phys_pc, phys_page2);
            tcg_tb_insert(tb);
        }
    }
    tu_trees_reset();
    return entry;
}
#endif