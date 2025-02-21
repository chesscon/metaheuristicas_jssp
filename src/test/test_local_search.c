#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../problem/job_shop_instance.h"
#include "../solution/permutations_machs_sol.h"
#include "../solution/evaluate_perms_machs_sol.h"
#include "../trajectory_search/trajectory_search.h"

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
  eval_solution(sol);

  printf("\n Planificación generada: \n");
  print_schedule(sol);

  printf("\n***** Makespan: %d ***** \n", sol->makespan);

  s_sol_perms_machs *local_opt = local_search_jssp(sol);

  printf("\n Solución Optimizada \n");
  print_sol_perms_machs(local_opt);
  printf("\n Evaluando... Optimizada \n");
  eval_solution(local_opt);
  printf("\n***** Makespan: %d ***** \n", local_opt->makespan);

  printf("\n Planificación generada: \n");
  print_schedule(local_opt);

  free_sol_perms_machs(local_opt);
  free_sol_perms_machs(sol);
  free_instance(inst);

  return 0;
}