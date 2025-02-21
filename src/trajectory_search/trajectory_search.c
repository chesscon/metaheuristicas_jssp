#include <stdio.h>
#include <stdlib.h>
#include "trajectory_search.h"
#include "../neighborhood/N1.h"

s_sol_perms_machs * local_search_jssp(s_sol_perms_machs * initial_sol) {
    s_sol_perms_machs *best = clone_sol_perms(initial_sol);
    s_sol_perms_machs *current = clone_sol_perms(initial_sol);

    s_neighborhood * neighborhood = allocate_neighborhood(current->inst->num_jobs * current->inst->num_machs);
    int best_index;

    int improve = 1;
    int iteration = 0;

    while (improve) {
        improve = 0;
        iteration++;

        generate_neighbors_n1(current, neighborhood);
        best_index = get_index_neighbor_best(current, neighborhood);
        apply_and_evaluate_move_N1(current, &neighborhood->neighbors[best_index]);

        printf("Iter: %d, best: %d, estimate_best_neighbor: %d, exact_best_neighbor: %d \n", 
            iteration, best->makespan, neighborhood->neighbors[best_index].eval, current->makespan
        );

        if (current->makespan < best->makespan) {
            improve = 1;
            copy_sol_perms(current, best);
        }
    };

    free_neighborhood(neighborhood);
    free_sol_perms_machs(current);

    return best;
}