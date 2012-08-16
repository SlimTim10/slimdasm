#ifndef _PELIB_C
#define _PELIB_C

#include <stdio.h>

#include "pe.h"

int parse_pe_header(PESTRUCT *p, FILE *fp, BYTE *buf) {
  // Get PE offset value from DOS stub
  fseek(fp, 0x3C, SEEK_SET);
  fgets(buf, 4, fp);

  // PE offset
  p->offset = lendian(buf, 4);

  // Get RVA of entry point
  fseek(fp, (p->offset + 40), SEEK_SET);
  fgets(buf, 4, fp);

  // RVA of entry point
  p->rvaep = lendian(buf, 4);

  // Get RVA of code section
  fseek(fp, (p->offset + 44), SEEK_SET);
  fgets(buf, 4, fp);

  // RVA of code section
  p->rvacode = lendian(buf, 4);

  // Get RVA of data section
  fseek(fp, (p->offset + 48), SEEK_SET);
  fgets(buf, 4, fp);

  // RVA of data section
  p->rvadata = lendian(buf, 4);

  // Get address of image base
  fseek(fp, (p->offset + 52), SEEK_SET);
  fgets(buf, 4, fp);

  // Address of image base
  p->base = lendian(buf, 4);

  // Get size of code section
  fseek(fp, (p->offset + 28), SEEK_SET);
  fgets(buf, 4, fp);

  // Size of code section
  p->codesize = lendian(buf, 4);

  return 0;
}

#endif
