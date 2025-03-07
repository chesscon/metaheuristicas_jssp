#include <stdlib.h>
#include <stdio.h>
#include "hga_crossover.h"

void copy_machine_configuration(s_sol_perms_machs *org, s_sol_perms_machs *dest, int idx_mach) {
    for (int i =0; i < org->inst->num_jobs; i++) {
        dest->machs[idx_mach][i].op = org->machs[idx_mach][i].op;
        dest->ops[dest->machs[idx_mach][i].op->id] = &dest->machs[idx_mach][i];
    }
}

void hga_crossover(
    s_sol_perms_machs *p1,
    s_sol_perms_machs *p2,
    s_sol_perms_machs *h1,
    s_sol_perms_machs *h2
) {
    for (int i =0; i < p1->inst->num_machs; i++) {
        if (rand() % 2 == 0) {
            copy_machine_configuration(p1, h1, i);
            copy_machine_configuration(p2, h2, i);
        } else {
            copy_machine_configuration(p1, h2, i);
            copy_machine_configuration(p2, h1, i);
        }
    }
}