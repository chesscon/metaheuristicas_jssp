#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../problem/job_shop_instance.h"
#include "../solution/permutations_machs_sol.h"
#include "../solution/evaluate_perms_machs_sol.h"
#include "../recombination/hga_crossover.h"
#include "../solution/repair_perms_sol.h"

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

  s_sol_perms_machs *p1 = make_random_feasible_solution_perms(inst);
  printf("\n Solución Generada P1 \n");
  print_sol_perms_machs(p1);
  eval_solution(p1);
  printf("\n***** Makespan P1: %d ***** \n", p1->makespan);

  s_sol_perms_machs *p2 = make_random_feasible_solution_perms(inst);
  printf("\n Solución Generada P2 \n");
  print_sol_perms_machs(p2);
  eval_solution(p2);
  printf("\n***** Makespan P2: %d ***** \n", p2->makespan);

  s_sol_perms_machs *h1 = allocate_sol_perms(inst);
  s_sol_perms_machs *h2 = allocate_sol_perms(inst);

  hga_crossover(p1, p2, h1, h2);

  printf("\n Solución Generada h1 \n");
  print_sol_perms_machs(h1);
  repair_solution(h1);
  printf("\n Solución Reparada h1 \n");
  print_sol_perms_machs(h1);

  eval_solution(h1);
  printf("\n***** Makespan H1: %d ***** \n", h1->makespan);

  printf("\n Solución Generada h2 \n");
  print_sol_perms_machs(h2);

  eval_solution(h2);
  printf("\n***** Makespan H2: %d ***** \n", h2->makespan);

  repair_solution(h2);
  printf("\n Solución Reparada h2 \n");
  print_sol_perms_machs(h2);
  eval_solution(h2);
  printf("\n***** Makespan H2: %d ***** \n", h2->makespan);
  print_schedule(h2);
  

  free_sol_perms_machs(p1);
  free_sol_perms_machs(p2);
  free_sol_perms_machs(h1);
  free_sol_perms_machs(h2);
  free_instance(inst);

  return 0;
}