#ifndef PERMUTATION_MACHS_SOL_H
#define PERMUTATION_MACHS_SOL_H

#include "../problem/job_shop_instance.h"

typedef struct {
    s_operacion *op;

    int seq_m;

    /** release date, inicio más temprano */
    int r;

    /** length tail, ruta más larga hacia el final (contando la op actual) */
    int q;

    /** length tail, ruta más larga hacia el final (sin contar la op actual) */
    int t;

} s_op_schedule;

typedef struct {
    // Datos del ejemplar
    s_jssp *inst;

    // Matriz con permutacioones de las operaciones en maquinas
    s_op_schedule ** machs;

    // lista de operaciones planificadas, indexadas por id
    s_op_schedule ** ops;

    int makespan;
} s_sol_perms_machs;


s_sol_perms_machs * allocate_sol_perms( s_jssp *inst );
void free_sol_perms_machs(s_sol_perms_machs *);

// TODO: Agregar métodos auxiliaras para copiar y clonas soluciones
// void copy_sol_perms(s_sol_perms_machs *src, s_sol_perms_machs *dst);
// s_sol_perms_machs * clone_sol_perms(s_sol_perms_machs *src);

// TODO: Agregar método para escribir solución a un archivo
// s_jssp * write_sol_perms_to_file(s_sol_perms_machs *sol, char *filename);

void print_operation_schedule(s_op_schedule *);

void print_sol_perms_machs(s_sol_perms_machs *);

s_sol_perms_machs * make_random_feasible_solution_perms( s_jssp *inst );

#endif