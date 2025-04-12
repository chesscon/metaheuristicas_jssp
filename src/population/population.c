#include <stdio.h>
#include <stdlib.h>
#include "population.h"
#include "../problem/job_shop_instance.h"
#include "../solution/permutations_machs_sol.h"
#include "../solution/evaluate_perms_machs_sol.h"
#include "../trajectory_search/tabu_search.h"

s_population * allocate_population( s_jssp *inst, int size ) {
    s_population *pob = NULL;

    pob = malloc(sizeof(s_population));
    pob->inds = malloc(sizeof(s_sol_perms_machs *)*size);
    pob->size = size;

    for(int i=0; i < size; i++) {
        pob->inds[i] = allocate_sol_perms(inst);
    }

    return pob;
}

void free_population(s_population *pob) {
    for(int i=0; i < pob->size; i++) {
        free_sol_perms_machs(pob->inds[i]);
    }
    free(pob->inds);
    free(pob);
}

s_population * make_random_population( s_jssp *inst, int size ) {
    s_population *pob = allocate_population(inst, size);

    for(int i =0; i < size; i++) {
        generate_random_feasible_solution_perm(inst, pob->inds[i]);
    }

    return pob;
}

int evaluate_population( s_population *pob ) {
    int idx_best = 0;

    for(int i =0; i < pob->size; i++) {
        eval_solution(pob->inds[i]);
        if (pob->inds[i]->makespan < pob->inds[idx_best]->makespan) {
            idx_best = i;
        }
    }

    return idx_best;
}

