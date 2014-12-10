#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "pe.h"
#include "utils.h"
#include "dasm.h"
#include "scan_codes.h"
#include "output.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		usage(argv[0]);
	}

	FILE *fin;	// Input file for disassembly

	if ((fin = fopen(argv[1], "rb")) == 0) {
		fprintf(stderr, "Error: could not open file\n");
		exit(1);
	}

	BYTE *fbuf;
	PESTRUCT *pe;

	fbuf = (BYTE *) malloc(256 * sizeof(BYTE));
	pe = (PESTRUCT *) malloc(sizeof(PESTRUCT));

	parse_pe_header(pe, fin, fbuf);

	printf("EP RVA: %.8X\n", pe->rvaep);
	printf("Code section RVA: %.8X\n", pe->rvacode);
	printf("Data section RVA: %.8X\n", pe->rvadata);
	printf("Image base: %.8X\n", pe->base);
	printf("Size of code section: %.8X\n", pe->codesize);

	/* Get size of headers to know where code section starts */
	fseek(fin, pe->offset + 84, SEEK_SET);
	fgets(fbuf, 4, fin);

	pe->codeoffset = lendian(fbuf, 4);
	printf("Code section offset: %.8X\n", pe->codeoffset);

	pe->oep = pe->base + pe->rvacode;
	printf("OEP address: %.8X\n", pe->oep);

	printf("\n");

	/* Get max offset from total file size */
	fseek(fin, 0L, SEEK_END);
	pe->maxoffset = ftell(fin);

	fseek(fin, pe->codeoffset, SEEK_SET);	// Go to start of code section

	DWORD len;
	DWORD cur_addr = pe->oep;	// First instruction at OEP
	DWORD addr;
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
			print_instr(fin, pe, &cur_addr);
			break;
		case ' ':	// Next 32 instructions
			print_ninstr(fin, pe, &cur_addr, 50);
			break;
		case 'o':	// Go back to OEP
			printf("\r \n");	// Clear line
			printf("EP RVA: %.8X\n", pe->rvaep);
			printf("Code section RVA: %.8X\n", pe->rvacode);
			printf("Data section RVA: %.8X\n", pe->rvadata);
			printf("Image base: %.8X\n", pe->base);
			printf("Size of code section: %.8X\n", pe->codesize);
			printf("Code section offset: %.8X\n", pe->codeoffset);
			printf("OEP address: %.8X\n", pe->oep);
			printf("\n");
			fseek(fin, pe->codeoffset, SEEK_SET);
			cur_addr = pe->oep;
			break;
		case 'g': {	// Go to specific address
			printf("\r \nGo to address: ");
			char getaddr[32];
			fgets(getaddr, sizeof(getaddr), stdin);
			addr = strtol(getaddr, NULL, 16);	// Parse input address
			if (!print_instr(fin, pe, &addr)) break;	// Print the first instruction
			cur_addr = addr;
			break;
		}
		case 'f': {	// Follow instruction at specific address
			printf("\r \nAddress of instruction to follow: ");
			char addrstr[32];
			fgets(addrstr, sizeof(addrstr), stdin);
			addr = strtol(addrstr, NULL, 16);
			char *instr = get_instr(fin, pe, addr);
			if (!instr) break;	// Error
			printf("%.8X\t%s\n", addr, instr);	// Print instruction to follow
			printf("\t\tv\n");
			addr = parse_addr(instr);
			if (!print_instr(fin, pe, &addr)) break;
			cur_addr = addr;
			free(instr);
			break;
		}
		case 'd': {	// Dump data at specific address
			printf("\r \nAddress to dump: ");
			char addrstr[32];
			fgets(addrstr, sizeof(addrstr), stdin);
			addr = strtol(addrstr, NULL, 16);
			if (in_code_section(pe, addr)) {
				printf("Valid!\n");
			} else {
				printf("Invalid...\n");
			}
			break;
		}
		case 'h':	// Show help
		case '?':
			print_help();
			break;

		// Special keys
		case SPECIAL_KEY:
			ch = _getch();
			switch (ch) {
			case KEY_DOWN:	// Next instruction
				print_instr(fin, pe, &cur_addr);
				break;
			case KEY_PGDN:	// Next 32 instructions
				print_ninstr(fin, pe, &cur_addr, 50);
				break;
			case KEY_HOME:	// Go back to OEP
				printf("\r \n");	// Clear line
				printf("EP RVA: %.8X\n", pe->rvaep);
				printf("Code section RVA: %.8X\n", pe->rvacode);
				printf("Data section RVA: %.8X\n", pe->rvadata);
				printf("Image base: %.8X\n", pe->base);
				printf("Size of code section: %.8X\n", pe->codesize);
				printf("Code section offset: %.8X\n", pe->codeoffset);
				printf("OEP address: %.8X\n", pe->oep);
				printf("\n");
				fseek(fin, pe->codeoffset, SEEK_SET);
				cur_addr = pe->oep;
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
