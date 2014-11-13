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
uint8_t valid_addr(PESTRUCT *pe, DWORD addr) {
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
	return (addr - pe->oep + pe->codeoffset);
}

/* Print usage */
void usage(char *prog) {
	fprintf(stderr, "usage: %s target\n", prog);
	exit(1);
}
