#include <stdio.h>

#include "output.h"
#include "defs.h"
#include "pe.h"

void print_help(void) {
	printf("\r \n");	// Clear line
	printf("q-Quit\n");
	printf("n-Next instruction\tspace-Next 50 instructions\n");
	printf("o-Go to OEP\n");
	printf("g-Go to address\n");
	printf("f-Follow instruction\n");
	printf("h-Show help\n\n");
}

/* Parse and print the instruction at address addr, then point addr to the following instruction */
void print_instr(FILE *fp, PESTRUCT *pe, DWORD *addr) {
	DWORD curpos = ftell(fp);	// Get current position in stream
	if (!valid_addr(pe, *addr)) {
		printf("ERROR: Address out of bounds\n");
		return;
	}
	printf("%.8X\t%s\n", *addr, parse_instr(fp, *addr));	// Parse and print instruction
	*addr += (ftell(fp) - curpos);
}

/* Parse and print the next n instructions, starting at address addr, then point addr to the following instruction */
void print_ninstr(FILE *fp, PESTRUCT *pe, DWORD *addr, int n) {
	int i;
	for (i = 0; i < n; i++) {
		print_instr(fp, pe, addr);
	}
}
