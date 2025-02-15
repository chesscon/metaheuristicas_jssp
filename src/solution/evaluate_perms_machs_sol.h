#ifndef EVALUATE_PERMS_MACHS_SOL_H
#define EVALUATE_PERMS_MACHS_SOL_H

#include "permutations_machs_sol.h"

s_op_schedule * get_predecesor_machine(s_sol_perms_machs *sol, s_op_schedule *op);
s_op_schedule * get_sucesor_machine(s_sol_perms_machs *sol, s_op_schedule *op);
s_op_schedule * get_predecesor_job(s_sol_perms_machs *sol, s_op_schedule *op);
s_op_schedule * get_sucesor_job(s_sol_perms_machs *sol, s_op_schedule *op);

void calculate_relase_times(s_sol_perms_machs *sol);

void calculate_length_tails(s_sol_perms_machs *sol);

void print_schedule(s_sol_perms_machs *sol);

#endif