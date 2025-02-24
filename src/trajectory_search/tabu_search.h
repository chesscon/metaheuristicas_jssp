#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include "../neighborhood/N1.h"
#include "../solution/permutations_machs_sol.h"

typedef struct {
    int mach;
    int job_a;
    int job_b;

    int last_tabu_it;
} s_tabu_move;

typedef struct {
    s_tabu_move *list;
    int size;

    int max_size;
} s_tabu_list;

s_tabu_list * allocate_tabu_list( int max_size );
void free_tabu_list(s_tabu_list * tabu_list);

int is_move_allowed(s_move *move, 
    s_tabu_list *tabu_list,
    int current_iteration);

int select_index_neighbor(int index_neighbor_best, s_neighborhood * neighborhood, int current_index );

void update_tabu_list(s_tabu_list *tabu_list, s_move move, int current_iteration, int min_tabu_tenure);

s_sol_perms_machs * tabu_search_jssp(
    s_sol_perms_machs * initial_sol, 
    int max_iters,
    int max_size_tabu,
    int min_tabu_tenure
);

#endif