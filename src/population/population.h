#ifndef POPULATION_H
#define POPULATION_H

#include "../problem/job_shop_instance.h"
#include "../solution/permutations_machs_sol.h"

typedef struct {
    s_sol_perms_machs **inds;

    int size;
} s_population;

s_population * allocate_population( s_jssp *inst, int size );

void free_population(s_population *pob);

s_population * make_random_population( s_jssp *inst, int size );

int evaluate_population( s_population *pob );

int improve_population( s_population *pob );

#endif