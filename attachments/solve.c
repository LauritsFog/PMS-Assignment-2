#include <stdlib.h>
#include <stdio.h>
#include "msptools.h"

int call_dgesv(array2d_t * A, array_t * b);

int main(int argc, char *argv[]) {

  if (argc != 4) {
    fprintf(stderr,"Usage: %s A b x\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* Insert your code here */

  return EXIT_SUCCESS;
}
