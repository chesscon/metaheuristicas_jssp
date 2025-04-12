#ifndef HGA_CROSSOVER_H
#define GENETICH_ALGORITHM_H

#include "../solution/permutations_machs_sol.h"

void copy_machine_configuration(s_sol_perms_machs *org, s_sol_perms_machs *dest, int num_mac);

void hga_crossover(
    s_sol_perms_machs *p1,
    s_sol_perms_machs *p2,
    s_sol_perms_machs *h1,
    s_sol_perms_machs *h2
);

#endif