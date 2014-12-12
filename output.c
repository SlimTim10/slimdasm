#include <stdio.h>
#include <stdlib.h>

#include "output.h"
#include "defs.h"
#include "pe.h"
#include "utils.h"

/* Show user interface help */
void print_help(void) {
	printf("\r \n");	// Clear line
	printf("q-Quit\n");
	printf("n-Next instruction\tspace-Next 32 instructions\n");
	printf("o-Go to OEP\n");
	printf("g-Go to address\n");
	printf("f-Follow instruction\n");
	printf("d-Dump\n");
	printf("h-Show help\n\n");
}

/* Parse and print the instruction at address addr, then point addr to the following instruction */
int print_instr(FILE *fp, PESTRUCT *pe, DWORD *addr) {
	if (!in_code_section(pe, *addr)) {
		printf("Address out of bounds\n\n");
		return 0;
	}

	DWORD curpos = addr_to_offset(pe, fp, *addr);	// Set current position in stream
	fseek(fp, curpos, SEEK_SET);
	char *str = (char *) parse_instr(fp, *addr);
	printf("%.8X\t%s\n", *addr, str);	// Parse and print instruction
	free(str);
	*addr += (ftell(fp) - curpos);	// Update address

	return 1;
}

/* Parse and print the next n instructions, given starting address, then point addr to the following instruction */
void print_ninstr(FILE *fp, PESTRUCT *pe, DWORD *addr, int n) {
	int i;
	for (i = 0; i < n; i++) {
		print_instr(fp, pe, addr);
	}
}

/* Print a dump of n bytes, given starting address */
void print_dump(FILE *fp, PESTRUCT *pe, DWORD addr, int n) {
	DWORD fpos = ftell(fp);	// Get current file position

	DWORD curpos = addr_to_offset(pe, fp, addr);
	fseek(fp, curpos, SEEK_SET);

	char ascii[DUMP_WIDTH+1];	// Also show ASCII characters where possible

	int i = 0;
	while (i < n && valid_addr(pe, fp, addr)) {
		printf("%.8X:", addr);	// Print address
		/* Print bytes in groups of 16 */
		int j;
		for (j = 0; j < DUMP_WIDTH && i < n && valid_addr(pe, fp, addr); j++, i++, addr++) {
			BYTE b = fgetc(fp);	// Get byte at address
			printf(" %.2X", b);	// Output byte as hex
			if (b >= ' ' && b <= '~') {	// Only show regular ASCII characters
				ascii[j] = b;
			} else {
				ascii[j] = '.';	// Dot denotes special character
			}
		}
		for (; j <= DUMP_WIDTH; j++) ascii[j] = 0x00;	// End string
		printf("    %s\n", ascii);
	}

	fseek(fp, fpos, SEEK_SET); // Restore previous file position
}

/* Print usage */
void usage(char *prog) {
	fprintf(stderr, "usage: %s target\n", prog);
	exit(1);
}
