#ifndef PERMUTATION_MACHS_SOL_H
#define PERMUTATION_MACHS_SOL_H

#include "../problem/job_shop_instance.h"

typedef struct {
    int num_machs;
    int num_jobs;

    int ** machs;
    
    int makespan;
} s_sol_perms_machs;


s_sol_perms_machs * allocate_sol_perms(int num_jobs, int num_machs);
void free_sol_perms_machs(s_sol_perms_machs *);

// TODO: Agregar métodos auxiliaras para copiar y clonas soluciones
// void copy_sol_perms(s_sol_perms_machs *src, s_sol_perms_machs *dst);
// s_sol_perms_machs * clone_sol_perms(s_sol_perms_machs *src);

// TODO: Agregar método para escribir solución a un archivo
// s_jssp * write_sol_perms_to_file(s_sol_perms_machs *sol, char *filename);

void print_sol_perms_machs(s_sol_perms_machs *);

s_sol_perms_machs * make_random_feasible_solution_perms( s_jssp *inst );

#endif