#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
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
int in_code_section(PESTRUCT *pe, DWORD addr) {
	DWORD minaddr = pe->imagebase;
	DWORD maxaddr = pe->imagebase + pe->rvacode + pe->codesize;

	if (addr < minaddr || addr > maxaddr) {
		return 0;
	} else {
		return 1;
	}
}

/* Return true iff the address is within the bounds of the entire file */
int valid_addr(PESTRUCT *pe, FILE *fp, DWORD addr) {
	if (addr_to_offset(pe, fp, addr) < pe->maxoffset) {
		return 1;
	} else {
		return 0;
	}
}

/* Return address converted to offset */
DWORD addr_to_offset(PESTRUCT *pe, FILE *fp, DWORD addr) {
	DWORD offset;

	if (addr < pe->oep) {
		offset = addr - pe->imagebase;
	} else {
		SECTSTRUCT *sect = (SECTSTRUCT *) malloc(sizeof(SECTSTRUCT));
		int i = 0;
		do {
			parse_section(fp, pe, sect, i);
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
char *get_instr(FILE *fp, PESTRUCT *pe, DWORD addr) {
	if (!valid_addr(pe, fp, addr)) {
		printf("Address out of bounds\n\n");
		return;
	}

	DWORD curpos = addr_to_offset(pe, fp, addr);	// Set current position in stream
	fseek(fp, curpos, SEEK_SET);

	return ((char *) parse_instr(fp, addr));
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
