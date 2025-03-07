#ifndef REPAIR_PERMS_SOL_H
#define REPAIR_PERMS_SOL_H

#include "permutations_machs_sol.h"

void initial_job_operations(s_sol_perms_machs *sol, s_op_schedule **job_ops, int * total, int * labaleds);

void initial_mach_operations(s_sol_perms_machs *sol, s_op_schedule **mach_ops, int * total, int * labaleds);

void intersection_ops(
    s_op_schedule **mach_ops, int total_mach_ops,
    s_op_schedule **K, int * total_intersection,
    int * labaleds
);

void diff_ops(s_op_schedule **src_ops, int * total_src, s_op_schedule **diff_ops, int total_diff );

void suc_job_operations(s_sol_perms_machs *sol, s_op_schedule **job_ops, int * total_job, s_op_schedule **K, int total_K, int * labaleds);
void suc_mach_operations(s_sol_perms_machs *sol, s_op_schedule **mach_ops, int * total_mach, s_op_schedule **K, int total_K, int * labaleds);

void rearrenge_random_operation( 
    s_sol_perms_machs *sol, 
    s_op_schedule **job_ops, int total_job_ops, 
    s_op_schedule **mach_ops, int total_mach_ops, 
    s_op_schedule **K, int * total_insersection, 
    int * labaleds
);

void repair_solution(s_sol_perms_machs *sol);

#endif