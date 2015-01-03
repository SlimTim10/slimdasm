#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "global.h"
#include "defs.h"
#include "pe.h"

/* Return a DWORD of the little endian value of an array of length
   count (max 4) */
DWORD lendian(BYTE *n, uint8_t count) {
	int ret = 0;
	int i, shift;

	count = min(count, 4);

	for (i = 0; i < count; i++) {
		shift = i * 8;
		ret |= (n[i] << shift);
	}

	return ret;
}

/* Return a string of the signed hexadecimal value of a byte */
char *sign8x(BYTE b) {
	char *ret = (char *) malloc(4 * sizeof(char));	// Return string

	if (b & 0x80) {
		sprintf(ret, "-%X", (BYTE) ((~b)+1));
	} else {
		sprintf(ret, "%X", b);
	}

	return ret;
}

/* Return true iff the address is within the bounds of the code section */
int in_code_section(DWORD addr) {
	DWORD minaddr = pe->imagebase;
	DWORD maxaddr = pe->imagebase + pe->rvacode + pe->codesize;

	if (addr < minaddr || addr > maxaddr) {
		return 0;
	} else {
		return 1;
	}
}

/* Return true iff the address is within the bounds of the entire file */
int valid_addr(DWORD addr) {
	if (addr_to_offset(addr) < pe->maxoffset) {
		return 1;
	} else {
		return 0;
	}
}

/* Return address converted to offset */
DWORD addr_to_offset(DWORD addr) {
	DWORD offset;

	if (addr < pe->oep) {
		offset = addr - pe->imagebase;
	} else {
		SECTSTRUCT *sect = (SECTSTRUCT *) malloc(sizeof(SECTSTRUCT));
		int i = 0;
		do {
			parse_section(sect, i);
			// DEBUGGING
			/* printf("section name: %s %d\n", sect->name, strcmp(sect->name, ".text")); */
			/* printf("section virtual address: %.8X\n", sect->va); */
			/* printf("section size: %.8X\n", sect->size); */
			/* printf("section offset: %.8X\n", sect->offset); */
			i++;
		} while (i < pe->numsects &&
				 ((addr - pe->imagebase) > (sect->va + sect->size)));

		// Found the section containing addr
		offset = addr - pe->imagebase - sect->va + sect->offset;

		free(sect);
	}

	return offset;
}

/* Parse and return the instruction at address addr */
char *get_instr(DWORD addr) {
	if (!valid_addr(addr)) {
		printf("Address out of bounds\n\n");
		return;
	}

	DWORD curpos = addr_to_offset(addr);	// Set current position in stream
	fseek(fin, curpos, SEEK_SET);

	return ((char *) parse_instr(addr));
}

/* Return first address in instruction string as DWORD */
/* Instruction must either be a jump (conditional or unconditional) or call */
/* Looking for format:
[JC].* (SHORT)? [0-9A-F]+
 */
DWORD parse_addr(char *instr) {
	char c = *instr++;
	if (!(c == 'J' || c == 'C')) {	// Mismatch
		return 0;
	}
	while (*instr && c != ' ') {	// Skip to space
		c = *instr++;
	}
	c = *instr++;
	if (c == 'S') {	// "SHORT"
		while (*instr && c != ' ') {	// Skip to space
			c = *instr++;
		}
		c = *instr++;
	}
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) {	// Start of address
		*instr--;
		return strtol(instr, NULL, 16);
	} else {
		return 0;	// Mismatch
	}
}

/* Return address of first occurrence of string by searching in sections: .data, .rdata, and .rsrc */
/* No unicode support */
DWORD find_string_addr(char *matchstr) {
	DWORD fpos = ftell(fin);	// Get current file position
	DWORD addr = 0;	// Address of string (0 = string not found)
	SECTSTRUCT *sect = (SECTSTRUCT *) malloc(sizeof(SECTSTRUCT));
	char readstr[STRLEN_MAX];
	DWORD cur_offset;

	/* Remove trailing newline and carriage return */
	char *cpos;
	if ((cpos = strchr(matchstr, '\n')) != NULL) {
		*cpos = '\0';
	}
	if ((cpos = strchr(matchstr, '\r')) != NULL) {
		*cpos = '\0';
	}

	readstr[0] = '\0';	// Start with empty string

	/* Loop through sections */
	int match_found = 0;
	int i;
	for (i = 0; i < pe->numsects && !match_found; i++) {
		parse_section(sect, i);
		/* Only search in .data, .rdata, and .rsrc */
		if ((strcmp(sect->name, ".data") == 0) || (strcmp(sect->name, ".rdata") == 0) || (strcmp(sect->name, ".rsrc") == 0)) {
			cur_offset = sect->offset;
			fseek(fin, cur_offset, SEEK_SET);
			/* Read and compare each string (separated by null bytes) */
			while (cur_offset < (sect->offset + sect->size - 0x100) && !match_found) {
				cur_offset = ftell(fin);
				/* Get string (the way fgets() should work) */
				char c = fgetc(fin);
				int k = 0;
				while(c != '\0' && k < STRLEN_MAX-1) {
					readstr[k++] = c;
					c = fgetc(fin);
				}
				readstr[k] = '\0';	// Null-terminate string
				if (strstr(readstr, matchstr)) {	// Check for substring match
					printf("%.8X\n", ftell(fin));///DEBUG
					addr = pe->imagebase + sect->va + (cur_offset - sect->offset);	// Convert offset to address
					match_found = 1;
				}
			}
		}
	}

	printf("%.8X\n", addr);

	free(sect);
	fseek(fin, fpos, SEEK_SET); // Restore previous file position

	return addr;
}
