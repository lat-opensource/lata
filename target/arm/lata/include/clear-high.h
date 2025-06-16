#ifndef _CLEAR_HIGH_H_
#define _CLEAR_HIGH_H_

#include "common.h"

extern bool clearGprHigh;

void set_w_write_flag(int i, int is_w);
void clear_gpr_high(int i);
#endif