#include <stdio.h>
#include <stdlib.h>

int lendian(unsigned char *, int);

typedef unsigned long DWORD;
typedef unsigned char BYTE;

struct pestruct {
  DWORD offset; /* Offset of PE header */
  DWORD rvaep; /* RVA of entry point */
  DWORD rvacode; /* RVA of code section */
  DWORD rvadata; /* RVA of data section */
};

int main(void) {
  FILE *fin;
  BYTE *fbuf;

  struct pestruct pe;

  fbuf = (char *)malloc(256 * sizeof(char));

  fin = fopen("test.exe", "rb");

  // Get PE offset value from DOS stub
  fseek(fin, 0x3C, SEEK_SET);
  fgets(fbuf, 4, fin);

  // PE offset
  pe.offset = lendian(fbuf, 4);
  printf("%.8x\n", pe.offset);

  fseek(fin, pe.offset, SEEK_SET);
  fgets(fbuf, 4, fin);

  /*  while (!feof(fin)) {
    fgets(fbuf, 10, fin);
    printf("%s", fbuf);
    }*/

  fclose(fin);
  free(fbuf);

  return 0;
}

/* Return a 32-bit integer of the little endian value of an array of length
count */
int lendian(unsigned char *n, int count) {
  int ret = 0;
  int i, shift;

  for (i = 0; i < count; i++) {
    shift = i * 8;
    ret |= (n[i] << shift);
  }

  return ret;
}
