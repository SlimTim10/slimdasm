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

	DWORD oep = pe->base + pe->rvacode;
	printf("OEP address: %.8X\n", oep);

	printf("\n");

	fseek(fin, codeoffset, SEEK_SET);	// Go to start of code section

	DWORD len;
	DWORD addr = oep;	// First instruction at OEP
	int i;
	int quit = 0;

	/* Start parsing and outputting 50 instructions at a time, waiting for user input after each block */
	while (!feof(fin) && !quit) {
		printf(":");

		/* Wait for user input */
		int ch = _getch();
		printf("\b");	// Erase character
		switch (ch) {
		case KEY_ESC:	// Quit
		case 'q':
			quit = 1;
			break;
		case 'n':	// Next instruction
			print_instr(fin, &addr);
			break;
		case ' ':	// Next 50 instructions
			print_ninstr(fin, &addr, 50);
			break;
		case 'o':	// Go back to OEP
			printf("\r \n");	// Clear line
			printf("EP RVA: %.8X\n", pe->rvaep);
			printf("Code section RVA: %.8X\n", pe->rvacode);
			printf("Data section RVA: %.8X\n", pe->rvadata);
			printf("Image base: %.8X\n", pe->base);
			printf("Size of code section: %.8X\n", pe->codesize);
			printf("Code section offset: %.8X\n", codeoffset);
			printf("OEP address: %.8X\n", oep);
			printf("\n");
			fseek(fin, codeoffset, SEEK_SET);
			addr = oep;
			break;
		case 'g': {	// Go to specific address
			printf("\r \nGo to address: ");
			char getaddr[32];
			fgets(getaddr, sizeof(getaddr), stdin);
			//TODO check bounds
			addr = strtol(getaddr, NULL, 16);
			fseek(fin, addr - oep + codeoffset, SEEK_SET);
			print_instr(fin, &addr);	// Print the first instruction
			break;
		}
		case 'h':	// Show help
		case '?':
			print_help();
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
			case KEY_HOME:	// Go back to OEP
				printf("\r \n");	// Clear line
				printf("EP RVA: %.8X\n", pe->rvaep);
				printf("Code section RVA: %.8X\n", pe->rvacode);
				printf("Data section RVA: %.8X\n", pe->rvadata);
				printf("Image base: %.8X\n", pe->base);
				printf("Size of code section: %.8X\n", pe->codesize);
				printf("Code section offset: %.8X\n", codeoffset);
				printf("OEP address: %.8X\n", oep);
				printf("\n");
				fseek(fin, codeoffset, SEEK_SET);
				addr = oep;
				break;
			case KEY_RIGHT: {	// Go to specific address
				printf("\r \nGo to address: ");
				char getaddr[32];
				fgets(getaddr, sizeof(getaddr), stdin);
				//TODO check bounds
				addr = strtol(getaddr, NULL, 16);
				fseek(fin, addr - oep + codeoffset, SEEK_SET);
				print_instr(fin, &addr);	// Print the first instruction
				break;
			}
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
