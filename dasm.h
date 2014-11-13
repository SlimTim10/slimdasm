#ifndef _DASM_H
#define _DASM_H

#include "defs.h"

char *parse_instr(FILE *, long int);
char *parse_modrm(FILE *, BYTE, char, char);
WORD get_word(FILE *);
DWORD get_dword(FILE *);

BYTE get_mod(BYTE);
BYTE get_regop(BYTE);
BYTE get_rm(BYTE);
char *reg_table(BYTE, char);
char *segreg_table(BYTE);
char *sib_to_str(BYTE);
char *group1_op(BYTE);
char *group2_op(BYTE);

#endif
