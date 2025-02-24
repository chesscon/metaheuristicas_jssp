#include <stdio.h>
#include <stdlib.h>
#include "N1.h"
#include "../utils.h"
#include "../solution/evaluate_perms_machs_sol.h"

s_neighborhood * allocate_neighborhood( int max_neighbors ) {
    s_neighborhood * neighborhood;
    neighborhood = malloc(sizeof(s_neighborhood));
    neighborhood->neighbors = malloc(sizeof(s_move)*max_neighbors);
    neighborhood->total_neighbors = 0;

    return neighborhood;
}

void free_neighborhood(s_neighborhood * neighborhood) {
    free(neighborhood->neighbors);
    free(neighborhood);
}

int is_critical_operation(int mach, int seq_mach, s_sol_perms_machs *sol) {
    return (sol->machs[mach][seq_mach].r +  sol->machs[mach][seq_mach].q) == sol->makespan;
}

void generate_neighbors_n1( s_sol_perms_machs *sol, s_neighborhood * neighborhood ) {
    neighborhood->total_neighbors = 0;
    for(int i=0; i< sol->inst->num_machs; i++) {
        for (int j=0; j < sol->inst->num_jobs - 1; j++) {
            if (
                (sol->machs[i][j].r +  sol->machs[i][j].q) == sol->makespan
                && (sol->machs[i][j+1].r +  sol->machs[i][j+1].q) == sol->makespan
            ) {
                neighborhood->neighbors[neighborhood->total_neighbors].a = &sol->machs[i][j];
                neighborhood->neighbors[neighborhood->total_neighbors].b = &sol->machs[i][j+1];
                neighborhood->total_neighbors++;
            }
        }
    }
}

int is_bloq_ini(int mach, int seq_mach, s_sol_perms_machs *sol) {
    return 
        is_critical_operation(mach, seq_mach, sol)
        && is_critical_operation(mach, seq_mach + 1, sol)
        && (seq_mach == 0 || !is_critical_operation(mach, seq_mach - 1, sol));
}

int is_bloq_end(int mach, int seq_mach, s_sol_perms_machs *sol) {
    return 
        is_critical_operation(mach, seq_mach, sol)
        && (seq_mach > 0 && is_critical_operation(mach, seq_mach - 1, sol))
        && (seq_mach == sol->inst->num_jobs -1 || !is_critical_operation(mach, seq_mach + 1, sol));
}

void generate_neighbors_n5( s_sol_perms_machs *sol, s_neighborhood * neighborhood ) {
    neighborhood->total_neighbors = 0;
    for(int i=0; i< sol->inst->num_machs; i++) { 
        for (int j=0; j < sol->inst->num_jobs - 1; j++) {
            if ( is_bloq_ini(i, j, sol)|| is_bloq_end(i, j+1, sol) )
             {
                neighborhood->neighbors[neighborhood->total_neighbors].a = &sol->machs[i][j];
                neighborhood->neighbors[neighborhood->total_neighbors].b = &sol->machs[i][j+1];
                neighborhood->total_neighbors++;
            }
        }
    }
}

int estimate_evaluation_neighbor( s_sol_perms_machs *sol, s_move * neighbor) {
    s_op_schedule *predecesor_mach_a = get_predecesor_machine(sol, neighbor->a);
    s_op_schedule *predecesor_job_a = get_predecesor_job(sol, neighbor->a);
    s_op_schedule *predecesor_job_b = get_predecesor_job(sol, neighbor->b);
    s_op_schedule *sucesor_job_a = get_sucesor_job(sol, neighbor->a);
    s_op_schedule *sucesor_job_b = get_sucesor_job(sol, neighbor->b);
    s_op_schedule *sucesor_mach_b = get_sucesor_job(sol, neighbor->b);

    int r_b_ = max_int( 
        predecesor_mach_a != NULL ? (predecesor_mach_a->r + predecesor_mach_a->op->time ) : 0,
        predecesor_job_b != NULL ? (predecesor_job_b->r + predecesor_job_b->op->time ) : 0
    );

    int r_a_ = max_int( 
        r_b_ + neighbor->b->op->time,
        predecesor_job_a != NULL ? (predecesor_job_a->r + predecesor_job_a->op->time ) : 0
    );

    int q_a_ = max_int( 
        sucesor_mach_b != NULL ? sucesor_mach_b->q : 0,
        sucesor_job_a != NULL ? sucesor_job_a->q : 0
    ) + neighbor->a->op->time;

    int q_b_ = max_int( 
        q_a_,
        sucesor_job_b != NULL ? sucesor_job_b->q : 0
    ) + neighbor->b->op->time;

    neighbor->eval = max_int( r_b_ + q_b_, r_a_ + q_a_ );

    return neighbor->eval;
}

int get_index_neighbor_best( s_sol_perms_machs *sol, s_neighborhood * neighborhood ) {
    int index_best = 0;

    for (int i =0; i < neighborhood->total_neighbors; i++) {
        neighborhood->neighbors[i].eval = estimate_evaluation_neighbor(sol, &neighborhood->neighbors[i]);
        
        if (neighborhood->neighbors[i].eval < neighborhood->neighbors[index_best].eval) {
            index_best = i;
        }
    }

    return index_best;
}

int apply_and_evaluate_move_N1( s_sol_perms_machs *sol, s_move * neigbor ) {
    int mach = neigbor->a->op->machine;
    int pos_a = neigbor->a->seq_m;
    int pos_b = neigbor->b->seq_m;

    s_operacion *tmp = sol->machs[mach][pos_a].op;
    sol->machs[mach][pos_a].op = sol->machs[mach][pos_b].op;
    sol->ops[sol->machs[mach][pos_a].op->id] = &sol->machs[mach][pos_a];

    sol->machs[mach][pos_b].op = tmp;
    sol->ops[sol->machs[mach][pos_b].op->id] = &sol->machs[mach][pos_b];

    int totalOps = sol->inst->num_jobs * sol->inst->num_machs;
    for(int i = 0; i < totalOps; i++) {
        sol->ops[i]->r = -1;
        sol->ops[i]->q = -1;
        sol->ops[i]->t = -1;
    }

    sol->makespan = eval_solution(sol);

    return sol->makespan;
}