#ifndef GENETICH_ALGORITHM_H
#define GENETICH_ALGORITHM_H

#include "../problem/job_shop_instance.h"
#include "../solution/permutations_machs_sol.h"
#include "../population/population.h"

s_sol_perms_machs * select_parent(s_population *pob);

void genera_hijos(s_population *pob, float pc, s_sol_perms_machs *h1, s_sol_perms_machs *h2);

void reemplaza_si_mejor_o_aleatorio(s_population *pob, s_sol_perms_machs *best, s_sol_perms_machs *h);

void update_population(s_population *pob, s_sol_perms_machs *best, s_sol_perms_machs *h1, s_sol_perms_machs *h2);

s_sol_perms_machs * genetic_algorithm_jssp(
    s_jssp *inst, 
    int size, 
    int max_iters,
    float pc,
    float pm
);

#endif