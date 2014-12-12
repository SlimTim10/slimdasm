#ifndef _UTILS_H
#define _UTILS_H

#include "defs.h"
#include "pe.h"

DWORD lendian(BYTE *, uint8_t);
char *sign8x(BYTE);
int in_code_section(PESTRUCT *, DWORD);
int valid_addr(PESTRUCT *, FILE *, DWORD);
DWORD addr_to_offset(PESTRUCT *, FILE *, DWORD);
char *get_instr(FILE *, PESTRUCT *, DWORD);
DWORD parse_addr(char *);

#endif
