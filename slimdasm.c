#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "pe.h"
#include "utils.h"
#include "dasm.h"

int main(int argc, char *argv[]) {
  FILE *fin;		// Global file pointer to input file for disassembly
  BYTE *fbuf;
  PESTRUCT *pe;

  fbuf = (BYTE *) malloc(256 * sizeof(char));
  pe = (PESTRUCT *) malloc(sizeof(PESTRUCT));

  if (argc < 2) {
    usage(argv[0]);
  }

  if ((fin = fopen(argv[1], "rb")) == 0) {
    fprintf(stderr, "Error: could not open file\n");
    exit(1);
  }

  parse_pe_header(pe, fin, fbuf);

  printf("EP RVA: %.8x\n", pe->rvaep);
  printf("Code section RVA: %.8x\n", pe->rvacode);
  printf("Data section RVA: %.8x\n", pe->rvadata);
  printf("Image base: %.8x\n", pe->base);
  printf("Size of code section: %.8x\n", pe->codesize);

  /* Get size of headers */
  fseek(fin, (pe->offset + 84), SEEK_SET);
  fgets(fbuf, 4, fin);

  DWORD codeoffset;
  codeoffset = lendian(fbuf, 4);
  printf("Code section offset: %.8x\n", codeoffset);

  printf("\n");

  fseek(fin, codeoffset, SEEK_SET);	// Go to start of code section

  long int len;
  DWORD addr = pe->base + pe->rvacode;
  char *instr;
  int i;
  for (i = 0; i < 24; i++) {
    len = ftell(fin);
    instr = parse_instr(fin, addr);
    printf("%.8x\t%s\n", addr, instr);
    len = ftell(fin) - len;
    addr += len;
  }

  /*  while (!feof(fin)) {
    fgets(fbuf, 10, fin);
    printf("%s", fbuf);
    }*/

  free(fbuf);
  free(pe);
  free(instr);

  fclose(fin);

  return 0;
}
