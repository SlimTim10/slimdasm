#ifndef _UTILS_H
#define _UTILS_H

#include "defs.h"
#include "pe.h"

DWORD lendian(BYTE *, uint8_t);
char *sign8x(BYTE);
int in_code_section(DWORD);
int valid_addr(DWORD);
DWORD addr_to_offset(DWORD);
char *get_instr(DWORD);
DWORD parse_addr(char *);
DWORD find_string_addr(char *);
void save_edits_to_file(FILE *, char *, DWORD, char *);

#endif
