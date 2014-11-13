#include <stdio.h>

#include "output.h"
#include "defs.h"

void print_help(void) {
	printf("\r \n");	// Clear line
	printf("esc-Quit\n");
	printf("down-Next instruction\tpgdn-Next 50 instructions\n");
	printf("up-Prev instruction\tpgup-Prev 50 instructions\n");
	printf("home-Go to OEP\n");
	printf("?-Show help\n\n");
}

/* Parse and print the instruction at address addr, then point addr to the following instruction */
void print_instr(FILE *fp, DWORD *addr) {
	DWORD curpos = ftell(fp);	// Get current position in stream
	printf("%.8X\t%s\n", *addr, parse_instr(fp, *addr));	// Parse and print instruction
	*addr += (ftell(fp) - curpos);
}

/* Parse and print the next n instructions, starting at address addr, then point addr to the following instruction */
void print_ninstr(FILE *fp, DWORD *addr, int n) {
	int i;
	for (i = 0; i < n; i++) {
		print_instr(fp, addr);
	}
}
