#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../problem/job_shop_instance.h"
#include "../solution/permutations_machs_sol.h"
#include "../solution/evaluate_perms_machs_sol.h"
#include "../neighborhood/N1.h"

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

  /* */
  printf("\n Planificación generada: \n");
  print_schedule(sol);

  printf("\n***** Makespan: %d ***** \n", sol->makespan);

  s_neighborhood * neighborhood = allocate_neighborhood(sol->inst->num_jobs * sol->inst->num_machs);
  generate_neighbors_n1(sol, neighborhood);

  int best_index = get_index_neighbor_best(sol, neighborhood);

  printf("Mejor vecino: [%d] = (M%d, (%d) J%d[%d], (%d) J%d[%d] ), eval=%d \n", 
    best_index, 
    neighborhood->neighbors[best_index].a->op->machine,
    
    neighborhood->neighbors[best_index].a->seq_m,
    neighborhood->neighbors[best_index].a->op->job,
    neighborhood->neighbors[best_index].a->op->seq,

    neighborhood->neighbors[best_index].b->seq_m,
    neighborhood->neighbors[best_index].b->op->job,
    neighborhood->neighbors[best_index].b->op->seq,

    neighborhood->neighbors[best_index].eval
  );

  apply_and_evaluate_move_N1(sol, &neighborhood->neighbors[best_index]);

  printf("\n Planificación generada: \n");
  print_schedule(sol);

  printf("\n***** Makespan: %d ***** \n", sol->makespan);

  free_neighborhood(neighborhood);
  free_sol_perms_machs(sol);
  free_instance(inst);

  return 0;
}