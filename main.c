#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "pe.h"
#include "utils.h"

int main(void) {
  FILE *fin;
  BYTE *fbuf;
  PESTRUCT *pe;

  fbuf = (BYTE *)malloc(256 * sizeof(char));
  pe = (PESTRUCT *)malloc(sizeof(PESTRUCT));

  fin = fopen("test.exe", "rb");

  parse_pe_header(pe, fin, fbuf);

  printf("EP RVA: %.8x\n", pe->rvaep);
  printf("Code section RVA: %.8x\n", pe->rvacode);
  printf("Data section RVA: %.8x\n", pe->rvadata);
  printf("Image base: %.8x\n", pe->base);
  printf("Size of code section: %.8x\n", pe->codesize);

  // Get size of headers
  fseek(fin, (pe->offset + 84), SEEK_SET);
  fgets(fbuf, 4, fin);

  DWORD codeoffset;
  codeoffset = lendian(fbuf, 4);
  printf("Code section offset: %.8x\n", codeoffset);

  printf("\n");

  // Go to start of code section
  fseek(fin, codeoffset, SEEK_SET);
  fgets(fbuf, 24, fin);
  int i;
  DWORD addr = pe->base + pe->rvacode;
  for (i = 0; i < 24; i++) {
    printf("%.8x\t%.2x\tINSTRUCTION\n", addr, fbuf[i]);
    addr++;
  }

  /*  while (!feof(fin)) {
    fgets(fbuf, 10, fin);
    printf("%s", fbuf);
    }*/

  fclose(fin);
  free(fbuf);

  return 0;
}
