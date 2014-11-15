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
int valid_addr(PESTRUCT *pe, DWORD addr) {
	DWORD minaddr = pe->base;
	DWORD maxaddr = pe->base + pe->rvacode + pe->codesize;

	if (addr < minaddr || addr > maxaddr) {
		return 0;
	} else {
		return 1;
	}
}

/* Return address converted to offset */
DWORD addr_to_offset(PESTRUCT *pe, DWORD addr) {
	if (addr < pe->oep) {
		return (addr - pe->base);
	} else {
		return (addr - pe->oep + pe->codeoffset);
	}
}

/* Parse and return the instruction at address addr */
char *get_instr(FILE *fp, PESTRUCT *pe, DWORD addr) {
	if (!valid_addr(pe, addr)) {
		printf("Address out of bounds\n");
		return;
	}

	DWORD curpos = addr_to_offset(pe, addr);	// Set current position in stream
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
