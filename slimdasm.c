#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "pe.h"
#include "utils.h"
#include "dasm.h"
#include "scan_codes.h"
#include "output.h"

int main(int argc, char *argv[]) {
	FILE *fin;	// Global file pointer to input file for disassembly
	BYTE *fbuf;
	PESTRUCT *pe;

	fbuf = (BYTE *) malloc(256 * sizeof(BYTE));
	pe = (PESTRUCT *) malloc(sizeof(PESTRUCT));

	if (argc < 2) {
		usage(argv[0]);
	}

	if ((fin = fopen(argv[1], "rb")) == 0) {
		fprintf(stderr, "Error: could not open file\n");
		exit(1);
	}

	parse_pe_header(pe, fin, fbuf);

	printf("EP RVA: %.8X\n", pe->rvaep);
	printf("Code section RVA: %.8X\n", pe->rvacode);
	printf("Data section RVA: %.8X\n", pe->rvadata);
	printf("Image base: %.8X\n", pe->base);
	printf("Size of code section: %.8X\n", pe->codesize);

	/* Get size of headers */
	fseek(fin, (pe->offset + 84), SEEK_SET);
	fgets(fbuf, 4, fin);

	DWORD codeoffset;
	codeoffset = lendian(fbuf, 4);
	printf("Code section offset: %.8X\n", codeoffset);

	printf("\n");

	fseek(fin, codeoffset, SEEK_SET);	// Go to start of code section

	DWORD len;
	DWORD addr = pe->base + pe->rvacode;	// First instruction at EP
	int i;
	int quit = 0;

	/* Start parsing and outputting 50 instructions at a time, waiting for user input after each block */
	while (!feof(fin) && !quit) {
		printf(":");

		/* Wait for user input */
		int ch = _getch();
		printf("\b");
		switch (ch) {
		case KEY_ESC:	// Quit
		case 'q':
			quit = 1;
			break;
		case SPECIAL_KEY:
			ch = _getch();
			switch (ch) {
			case KEY_DOWN:	// Next instruction
				print_instr(fin, &addr);
				break;
			case KEY_PGDN:	// Next 50 instructions
				print_ninstr(fin, &addr, 50);
				break;
			case KEY_HOME:	// Go back to EP
				fseek(fin, codeoffset, SEEK_SET);
				addr = pe->base + pe->rvacode;
				/* Print the first instruction */
				print_instr(fin, &addr);
				break;
			default:
				printf("0x%X\n", ch);	// Debugging
				break;
			}
			break;
		default:
			printf("0x%X\n", ch);	// Debugging
			break;
		}
	}

	free(fbuf);
	free(pe);

	fclose(fin);

	return 0;
}
