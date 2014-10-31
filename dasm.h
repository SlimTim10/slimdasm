#ifndef _DASMLIB_H
#define _DASMLIB_H

#include "defs.h"

char *parse_instr(FILE *, long int);
char *parse_modrm(FILE *, BYTE, char, char);
BYTE get_mod(BYTE);
BYTE get_regop(BYTE);
BYTE get_rm(BYTE);
char *reg_table(BYTE, char);
char *sib_to_str(BYTE);

#endif
