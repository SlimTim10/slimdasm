#ifndef _PE_H
#define _PE_H

#include <stdio.h>

#include "defs.h"

typedef struct {
	DWORD offset;	// Offset of PE header
	WORD numsects;	// Number of sections
	WORD peheadersize;	// Size of PE header
	DWORD rvaep;	// RVA of entry point
	DWORD rvacode;	// RVA of code section
	DWORD rvadata;	// RVA of data section
	DWORD imagebase;	// Address of image base
	DWORD codesize;	// Size of code section
	DWORD codeoffset;	// Code section offset
	DWORD oep;	// Original entry point address
	DWORD maxoffset;	// Maximum offset (total file size)
} PESTRUCT;

typedef struct {
	;
} SECTSTRUCT;

void parse_pe_header(PESTRUCT *, FILE *, BYTE *);
void parse_section(SECTSTRUCT *, FILE *, BYTE *, int);

#endif
