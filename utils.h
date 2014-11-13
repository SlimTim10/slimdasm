#ifndef _UTILS_H
#define _UTILS_H

#include "defs.h"
#include "pe.h"

DWORD lendian(BYTE *, uint8_t);
char *sign8x(BYTE);
uint8_t valid_addr(PESTRUCT *, DWORD);
DWORD addr_to_offset(PESTRUCT *, DWORD);
void usage(char *);

#endif
