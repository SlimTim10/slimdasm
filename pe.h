#ifndef _PELIB_H
#define _PELIB_H

#include <stdio.h>

#include "defs.h"

typedef struct {
	DWORD offset; /* Offset of PE header */
	DWORD rvaep; /* RVA of entry point */
	DWORD rvacode; /* RVA of code section */
	DWORD rvadata; /* RVA of data section */
	DWORD base; /* Address of image base */
	DWORD codesize; /* Size of code section */
} PESTRUCT;

int parse_pe_header(PESTRUCT *, FILE *, BYTE *);

#endif
