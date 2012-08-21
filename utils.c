#ifndef _UTILSLIB_C
#define _UTILSLIB_C

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "defs.h"

/* Return a 32-bit integer of the little endian value of an array of length
count (max 4) */
unsigned long lendian(unsigned char *n, int count) {
  int ret = 0;
  int i, shift;

  count = min(count, 4);

  for (i = 0; i < count; i++) {
    shift = i * 8;
    ret |= (n[i] << shift);
  }

  return ret;
}

/* Print usage */
void usage(char *prog) {
  fprintf(stderr, "usage: %s target\n", prog);
  exit(1);
}

#endif
