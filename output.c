#include <stdio.h>
#include <stdlib.h>

#include "output.h"
#include "defs.h"
#include "pe.h"

/* Show user interface help */
void print_help(void) {
	printf("\r \n");	// Clear line
	printf("q-Quit\n");
	printf("n-Next instruction\tspace-Next 32 instructions\n");
	printf("o-Go to OEP\n");
	printf("g-Go to address\n");
	printf("f-Follow instruction\n");
	printf("h-Show help\n\n");
}

/* Parse and print the instruction at address addr, then point addr to the following instruction */
void print_instr(FILE *fp, PESTRUCT *pe, DWORD *addr) {
	if (!valid_addr(pe, *addr)) {
		printf("Address out of bounds\n");
		return;
	}

	DWORD curpos = addr_to_offset(pe, *addr);	// Set current position in stream
	fseek(fp, curpos, SEEK_SET);
	char *str = (char *) parse_instr(fp, *addr);
	printf("%.8X\t%s\n", *addr, str);	// Parse and print instruction
	free(str);
	*addr += (ftell(fp) - curpos);	// Update address
}

/* Parse and print the next n instructions, given starting address, then point addr to the following instruction */
void print_ninstr(FILE *fp, PESTRUCT *pe, DWORD *addr, int n) {
	int i;
	for (i = 0; i < n; i++) {
		print_instr(fp, pe, addr);
	}
}
