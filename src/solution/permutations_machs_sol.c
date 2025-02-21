#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "permutations_machs_sol.h"
#include "../utils.h"


s_sol_perms_machs * allocate_sol_perms( s_jssp *inst ) {
    s_sol_perms_machs *sol = NULL;
    sol = malloc( sizeof(s_sol_perms_machs) );
    sol->inst = inst;
    sol->machs = malloc( sizeof(s_op_schedule *)*inst->num_machs );
    for(int i =0; i < inst->num_machs; i++) {
        sol->machs[i] = malloc( sizeof(s_op_schedule)*inst->num_jobs );
    }
    sol->ops = malloc( sizeof( s_op_schedule *)*(inst->num_jobs*inst->num_machs));
    sol->makespan = INT_MAX;

    return sol;
}

void free_sol_perms_machs(s_sol_perms_machs *sol) {
    free(sol->ops);
    for(int i =0; i < sol->inst->num_machs; i++) {
        free(sol->machs[i]);
    }
    free(sol->machs);
    free(sol);
}

void print_sol_perms_machs(s_sol_perms_machs *sol) {
    for (int i =0; i < sol->inst->num_machs; i++) {
        printf("M %d [ ", i);
        for (int j=0; j < sol->inst->num_jobs; j++) { 
            printf("%d, ", sol->machs[i][j].op->job);
        }
        printf("]\n");
    }
}

void print_operation_schedule(s_op_schedule *op) {
    if (op != NULL) {
        printf("( J%d[%d], M%d[%d], r=%d, d=%d) ", op->op->job, op->op->seq, op->op->machine, op->seq_m, op->r, op->op->time);
    } else{
        printf(" - ");
    }
}

s_sol_perms_machs * make_random_feasible_solution_perms(s_jssp *inst) {
    s_sol_perms_machs * sol = NULL;
    sol = allocate_sol_perms(inst);
    // Generamos contadores para considerar la planficacion en cada maquina
    int machs_free_position[inst->num_machs];
    for (int i = 0; i < inst->num_machs; i++) {
        machs_free_position[i] = 0;
    }

    // Calculamos las operaciones iniciales de cada job:
    s_operacion * availables[inst->num_jobs]; 
    for (int i =0; i < inst->num_jobs; i++) {
        availables[i] = &inst->jobs[i][0];
    }
    int total_availables = inst->num_jobs;
    
    
    int tmp_random;
    int current_free_position_mach;
    s_operacion * op_random;

    int k=0;

    while (total_availables > 0) {
        // 1. elegir una operacion aleatoria de las disponibles:
        tmp_random = rand() % total_availables;
        op_random = availables[tmp_random];

        // 2. planificarla
        current_free_position_mach = machs_free_position[op_random->machine];
        sol->machs[op_random->machine][current_free_position_mach].op = op_random;
        sol->machs[op_random->machine][current_free_position_mach].seq_m = current_free_position_mach;
        sol->machs[op_random->machine][current_free_position_mach].r = -1;
        sol->machs[op_random->machine][current_free_position_mach].q = -1;
        sol->machs[op_random->machine][current_free_position_mach].t = -1;
        sol->ops[op_random->id] = &sol->machs[op_random->machine][current_free_position_mach];

        machs_free_position[op_random->machine]++;

        // 3. Si tiene una operacion siguiente, reemplazarla por la actual
        if (op_random->seq < inst->num_machs-1) {
            // obtenemos la siguiente operacion:
            availables[tmp_random] =  &inst->jobs[op_random->job][op_random->seq+1];
        } else { //  - Si ya no hay siguiente, eliminar la actual y actualizar total_availables
            availables[tmp_random] = availables[total_availables - 1];
            total_availables--;
        }
    }

    return sol;
}




