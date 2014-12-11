#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "defs.h"
#include "pe.h"

void print_help(void);
int print_instr(FILE *, PESTRUCT *, DWORD *);
void print_ninstr(FILE *, PESTRUCT *, DWORD *, int);
void print_dump(FILE *, PESTRUCT *, DWORD, int);
void usage(char *);

#endif
