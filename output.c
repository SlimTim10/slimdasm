#include <stdio.h>
#include <stdlib.h>

#include "output.h"
#include "global.h"
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
	printf("s-Search for string\n");
	printf("h-Show help\n\n");
}

/* Parse and print the instruction at address addr, then point addr to the following instruction */
int print_instr(DWORD *addr) {
	if (!in_code_section(*addr)) {
		printf("Address out of bounds\n\n");
		return 0;
	}

	DWORD curpos = addr_to_offset(*addr);	// Set current position in stream
	fseek(fin, curpos, SEEK_SET);
	char *str = (char *) parse_instr(*addr);
	printf("%.8X\t%s\n", *addr, str);	// Parse and print instruction
	free(str);
	*addr += (ftell(fin) - curpos);	// Update address

	return 1;
}

/* Parse and print the next n instructions, given starting address, then point addr to the following instruction */
void print_ninstr(DWORD *addr, int n) {
	int i;
	for (i = 0; i < n; i++) {
		print_instr(addr);
	}
}

/* Print a dump of n bytes, given starting address */
void print_dump(DWORD addr, int n) {
	DWORD fpos = ftell(fin);	// Get current file position

	DWORD curpos = addr_to_offset(addr);
	fseek(fin, curpos, SEEK_SET);

	char ascii[DUMP_WIDTH+1];	// Also show ASCII characters where possible

	int i = 0;
	while (i < n && valid_addr(addr)) {
		printf("%.8X:", addr);	// Print address
		/* Print bytes in groups of 16 */
		int j;
		for (j = 0; j < DUMP_WIDTH && i < n && valid_addr(addr); j++, i++, addr++) {
			BYTE b = fgetc(fin);	// Get byte at address
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

	fseek(fin, fpos, SEEK_SET); // Restore previous file position
}

/* Print a dump of a null-terminated string at starting address */
void print_dump_str(DWORD addr) {
	DWORD fpos = ftell(fin);	// Get current file position

	DWORD curpos = addr_to_offset(addr);
	fseek(fin, curpos, SEEK_SET);

	char ascii[DUMP_WIDTH+1];	// Also show ASCII characters where possible

	BYTE b = fgetc(fin);	// Get byte at address
	while (b != '\0' && valid_addr(addr)) {
		printf("%.8X:", addr);	// Print address
		/* Print bytes in groups of 16 */
		int j;
		for (j = 0; j < DUMP_WIDTH && b != '\0' && valid_addr(addr); j++, addr++) {
			printf(" %.2X", b);	// Output byte as hex
			if (b >= ' ' && b <= '~') {	// Only show regular ASCII characters
				ascii[j] = b;
			} else {
				ascii[j] = '.';	// Dot denotes special character
			}
			b = fgetc(fin);	// Get byte at address
		}
		for (; j <= DUMP_WIDTH; j++) ascii[j] = 0x00;	// End string
		printf("    %s\n", ascii);
	}

	fseek(fin, fpos, SEEK_SET); // Restore previous file position
}

/* Print usage */
void usage(char *prog) {
	fprintf(stderr, "usage: %s target\n", prog);
	exit(1);
}
