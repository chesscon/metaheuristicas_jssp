#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "job_shop_instance.h"

int main(int argc, char *argv[]) {
  
  char *filename = argv[1];
  int tipo = argc > 2 ? atoi(argv[2]) : 1;

  printf("Leyendo datos del ejemplar: %s \n", filename);
  
  s_jssp *inst = NULL;
  inst = make_from_file(filename);

  /* */
  printf("Ejemplar leido: %s \n", filename);
  print_instance(inst);

  free_instance(inst);

  return 0;
}