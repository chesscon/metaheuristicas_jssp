#ifndef N1_H
#define N1_H

#include "../solution/permutations_machs_sol.h"

typedef struct {
    s_op_schedule *a;
    s_op_schedule *b;

    int eval;
} s_move;


typedef struct {
    s_move * neighbors;

    int total_neighbors;
} s_neighborhood;

s_neighborhood * allocate_neighborhood( int max_neighbors );
void free_neighborhood(s_neighborhood *neighborhood);

/* N1 */
int is_critical_operation(int mach, int seq_mach, s_sol_perms_machs *sol);

void generate_neighbors_n1( s_sol_perms_machs *sol, s_neighborhood * neighborhood );

/* N5 */

void generate_neighbors_n5( s_sol_perms_machs *sol, s_neighborhood * neighborhood );

int is_bloq_ini(int mach, int seq_mach, s_sol_perms_machs *sol);

int is_bloq_end(int mach, int seq_mach, s_sol_perms_machs *sol);

int estimate_evaluation_neighbor( s_sol_perms_machs *sol, s_move * neighbor);

int get_index_neighbor_best( s_sol_perms_machs *sol, s_neighborhood * neighborhood );

int apply_and_evaluate_move_N1( s_sol_perms_machs *sol, s_move * neigbor );

#endif

