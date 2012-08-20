#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *op[2];
  op[0] = (char *)malloc(4 * sizeof(char));
  //  op[1] = (char *)malloc(1 * sizeof(char));
  op[0] = "PUSH";
  *op[1] = 1;
  printf("%s %d\n", op[0], *op[1]);

  return 0;
}
