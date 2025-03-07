#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../problem/job_shop_instance.h"
#include "../solution/permutations_machs_sol.h"
#include "../solution/evaluate_perms_machs_sol.h"
#include "../population_search/genetic_algorithm.h"

int main(int argc, char *argv[]) {
  
  char *filename = argv[1];

  printf("Leyendo datos del ejemplar: %s \n", filename);

  //unsigned int seed = (unsigned int) time(NULL);
  unsigned int seed = 1;
  srand(seed);
  printf("SEED: %u \n", seed);
  
  s_jssp *inst = NULL;
  inst = make_instance_from_file(filename);

  int tipo = argc > 2 ? atoi(argv[2]) : 0;
  int max_iters = argc > 3 ? atoi(argv[3]) : 1000;
  int pob_size = argc > 4 ? atoi(argv[4]) : 30;
  float pc = argc > 5 ? atof(argv[5]) : 0.8;
  float pm = argc > 6 ? atof(argv[6]) : 1.0/pob_size;

  printf("\n **** PARAMETROS AG: ***** \n");
  printf("Max ITERS: %d \n", max_iters);
  printf("Tam POB: %d \n", pob_size);
  printf("Prob Cruza: %.3f \n", pc);
  printf("Prob Mutacion: %.3f \n", pm);

  s_sol_perms_machs *sol = genetic_algorithm_jssp(inst, pob_size, max_iters, pc, pm);
  printf("\n Solución Optimizada \n");
  print_sol_perms_machs(sol);
  printf("\n***** Makespan: %d ***** \n", sol->makespan);

  printf("\n Planificación generada: \n");
  print_schedule(sol);

  free_sol_perms_machs(sol);
  free_instance(inst);

  return 0;
}