#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../problem/job_shop_instance.h"
#include "../solution/permutations_machs_sol.h"
#include "../solution/evaluate_perms_machs_sol.h"

int main(int argc, char *argv[]) {
  
  char *filename = argv[1];
  int tipo = argc > 2 ? atoi(argv[2]) : 1;

  printf("Leyendo datos del ejemplar: %s \n", filename);

  //unsigned int seed = (unsigned int) time(NULL);
  unsigned int seed = 1;
  srand(seed);
  printf("SEED: %u \n", seed);
  
  s_jssp *inst = NULL;
  inst = make_instance_from_file(filename);

  s_sol_perms_machs *sol = make_random_feasible_solution_perms(inst);
  printf("\n Solución Generada \n");
  print_sol_perms_machs(sol);
  

  printf("\nCalculando: release times\n");
  calculate_relase_times(sol);

  printf("\nCalculando: length tails\n");
  calculate_length_tails(sol);

  /* */
  printf("\n Planificación generada: \n");
  print_schedule(sol);

  printf("\n***** Makespan: %d ***** \n", sol->makespan);

  free_sol_perms_machs(sol);
  free_instance(inst);

  return 0;
}