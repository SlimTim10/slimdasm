#include <stdio.h>

#include "output.h"
#include "defs.h"

/* Parse and print the next instruction */
DWORD next_instr(FILE *fp, DWORD addr) {
	DWORD curpos = ftell(fp);	// Get current position in stream
	printf("%.8X\t%s\n", addr, parse_instr(fp, addr));	// Parse and print instruction
	return (ftell(fp) - curpos);
}
