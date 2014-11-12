#ifndef _UTILSLIB_C
#define _UTILSLIB_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "defs.h"

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

/* Print usage */
void usage(char *prog) {
	fprintf(stderr, "usage: %s target\n", prog);
	exit(1);
}

#endif
