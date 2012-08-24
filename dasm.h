#ifndef _DASMLIB_H
#define _DASMLIB_H

#include "defs.h"

char *parse_instr(FILE *, long int);
char *reg_table(BYTE, char);
char *sib_to_str(BYTE);

#endif
