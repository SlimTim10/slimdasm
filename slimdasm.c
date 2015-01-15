#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "defs.h"
#include "pe.h"
#include "utils.h"
#include "dasm.h"
#include "scan_codes.h"
#include "output.h"

/* Global variables */
FILE *fin;	// Input file for disassembly
PESTRUCT *pe;	// PE info about file

int main(int argc, char *argv[]) {
	if (argc < 2) {
		usage(argv[0]);
	}

	if ((fin = fopen(argv[1], "rb")) == 0) {
		fprintf(stderr, "Error: could not open file\n");
		exit(1);
	}

	BYTE *fbuf = (BYTE *) malloc(16 * sizeof(BYTE));
	pe = (PESTRUCT *) malloc(sizeof(PESTRUCT));

	parse_pe_header();

	printf("EP RVA: %.8X\n", pe->rvaep);
	printf("Code section RVA: %.8X\n", pe->rvacode);
	printf("Data section RVA: %.8X\n", pe->rvadata);
	printf("Image base: %.8X\n", pe->imagebase);
	printf("Size of code section: %.8X\n", pe->codesize);

	/* Get size of headers to know where code section starts */
	fseek(fin, pe->offset + 84, SEEK_SET);
	fgets(fbuf, 4, fin);
	pe->codeoffset = lendian(fbuf, 4);
	printf("Code section offset: %.8X\n", pe->codeoffset);

	/* Get OEP address */
	pe->oep = pe->imagebase + pe->rvacode;
	printf("OEP address: %.8X\n", pe->oep);

	printf("\n");	// Formatting

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
			print_instr(&cur_addr);
			break;
		case ' ':	// Next 32 instructions
			print_ninstr(&cur_addr, 50);
			break;
		case 'o':	// Go back to OEP
			printf("\r \n");	// Clear line
			printf("EP RVA: %.8X\n", pe->rvaep);
			printf("Code section RVA: %.8X\n", pe->rvacode);
			printf("Data section RVA: %.8X\n", pe->rvadata);
			printf("Image base: %.8X\n", pe->imagebase);
			printf("Size of code section: %.8X\n", pe->codesize);
			printf("Code section offset: %.8X\n", pe->codeoffset);
			printf("OEP address: %.8X\n", pe->oep);
			printf("\n");
			fseek(fin, pe->codeoffset, SEEK_SET);
			cur_addr = pe->oep;
			break;
		case 'g': {	// Go to specific address
			printf("\r \n");	// Clear line
			printf("Go to address: ");
			char getaddr[32];
			fgets(getaddr, sizeof(getaddr), stdin);	// Get input
			if (getaddr[0] == '\n' || getaddr[0] == '\r') { printf("\n"); break; }	// Blank input (cancel instruction)
			addr = strtol(getaddr, NULL, 16);	// Parse input address
			if (!print_instr(&addr)) break;	// Print the first instruction
			cur_addr = addr;
			break;
		}
		case 'f': {	// Follow instruction at specific address
			printf("\r \n");	// Clear line
			printf("Address of instruction to follow: ");
			char addrstr[32];
			fgets(addrstr, sizeof(addrstr), stdin);	// Get input
			if (addrstr[0] == '\n' || addrstr[0] == '\r') { printf("\n"); break; }	// Blank input (cancel instruction)
			addr = strtol(addrstr, NULL, 16);
			char *instr = get_instr(addr);
			if (!instr) break;	// Error
			printf("%.8X\t%s\n", addr, instr);	// Print instruction to follow
			printf("\t\tv\n");
			addr = parse_addr(instr);
			if (!print_instr(&addr)) break;
			cur_addr = addr;
			free(instr);
			break;
		}
		case 'd': {	// Dump data at specific address
			printf("\r \n");	// Clear line
			printf("Address to dump: ");
			char addrstr[32];
			fgets(addrstr, sizeof(addrstr), stdin);	// Get input
			if (addrstr[0] == '\n' || addrstr[0] == '\r') { printf("\n"); break; }	// Blank input (cancel instruction)
			addr = strtol(addrstr, NULL, 16);
			if (!valid_addr(addr)) {
				printf("Address out of bounds\n");
				break;
			}
			printf("Number of bytes to dump (default 16): ");
			char bytesstr[3];
			fgets(bytesstr, sizeof(bytesstr), stdin);
			int bytes = strtol(bytesstr, NULL, 10);
			if (bytes == 0) bytes = 16;
			if (bytes > DUMP_MAX) {
				printf("Too high\n");
				break;
			}
			print_dump(addr, bytes);
			printf("\n");	// Formatting
			break;
		}
		case 's': {	// Search for a string in .data, .rdata, and .rsrc sections (no unicode support)
			printf("\r \n");	// Clear line
			printf("Search for string: ");
			char searchstr[STRLEN_MAX];
			fgets(searchstr, sizeof(searchstr), stdin);	// Get input
			if (searchstr[0] == '\n' || searchstr[0] == '\r') { printf("\n"); break; }	// Blank input (cancel instruction)
			addr = find_string_addr(searchstr);	// Find address of string
			if (addr == 0) {
				printf("String not found\n");
			} else {
				print_dump_str(addr);	// Print dump of matched string
			}
			printf("\n");	// Formatting
			break;
		}
		case 'e': {	// Edit binary
			printf("\r \n");	// Clear line
			printf("Starting address for editing: ");
			char addrstr[32];
			fgets(addrstr, sizeof(addrstr), stdin);	// Get input
			if (addrstr[0] == '\n' || addrstr[0] == '\r') { printf("\n"); break; }	// Blank input (cancel instruction)
			addr = strtol(addrstr, NULL, 16);
			if (!valid_addr(addr)) {
				printf("Address out of bounds\n");
				break;
			}
			printf("Input hex bytes: ");
			char bytestr[STRLEN_MAX];
			fgets(bytestr, sizeof(bytestr), stdin);	// Get input
			///TODO
			char *backup_file = (char *) malloc((sizeof(argv[1]) * sizeof(char)) + 5);
			backup_file = strcat(argv[1], ".bak");
			save_backup_file(fin, backup_file);
			printf("Backup file %s saved\n\n", backup_file);
			free(backup_file);
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
				print_instr(&cur_addr);
				break;
			case KEY_PGDN:	// Next 32 instructions
				print_ninstr(&cur_addr, 50);
				break;
			case KEY_HOME:	// Go back to OEP
				printf("\r \n");	// Clear line
				printf("EP RVA: %.8X\n", pe->rvaep);
				printf("Code section RVA: %.8X\n", pe->rvacode);
				printf("Data section RVA: %.8X\n", pe->rvadata);
				printf("Image base: %.8X\n", pe->imagebase);
				printf("Size of code section: %.8X\n", pe->codesize);
				printf("Code section offset: %.8X\n", pe->codeoffset);
				printf("OEP address: %.8X\n", pe->oep);
				printf("\n");
				fseek(fin, pe->codeoffset, SEEK_SET);
				cur_addr = pe->oep;
				break;
			default:
				//printf("0x%X\n", ch);	// DEBUGGING
				break;
			}
			break;

		default:
			//printf("0x%X\n", ch);	// DEBUGGING
			break;
		}

		fflush(stdin);
	}

	free(fbuf);
	free(pe);

	fclose(fin);

	return 0;
}
