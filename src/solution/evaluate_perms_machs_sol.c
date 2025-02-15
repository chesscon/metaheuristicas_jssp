#include <stdio.h>
#include "permutations_machs_sol.h"
#include "evaluate_perms_machs_sol.h"
#include "../utils.h"

s_op_schedule * get_predecesor_machine(s_sol_perms_machs *sol, s_op_schedule *op) {
    if (op != NULL && op->seq_m  > 0) {
        return &sol->machs[op->op->machine][op->seq_m-1];
    }
    return NULL;
}

s_op_schedule * get_sucesor_machine(s_sol_perms_machs *sol, s_op_schedule *op) {
    if (op != NULL && op->seq_m < sol->inst->num_jobs - 1) {
        return &sol->machs[op->op->machine][op->seq_m+1];
    }
    return NULL;
}

s_op_schedule * get_predecesor_job(s_sol_perms_machs *sol, s_op_schedule *op) {
    if (op != NULL && op->op->seq > 0 ) {
        return sol->ops[ op->op->id - 1];
    }
    return NULL;
}

s_op_schedule * get_sucesor_job(s_sol_perms_machs *sol, s_op_schedule *op) {
    if (op != NULL && op->op->seq < sol->inst->num_machs - 1 ) {
        return sol->ops[ op->op->id + 1];
    }
    return NULL;
}

void calculate_relase_times(s_sol_perms_machs *sol) {

    s_op_schedule * availables[sol->inst->num_jobs + 1]; 
    int total_availables = 0;

    s_operacion *job_op_current;
    s_op_schedule * mach_op_current;

    s_op_schedule *predecesor_job;
    s_op_schedule * predecesor_mach;

    s_op_schedule *sucesor_job;
    s_op_schedule * sucesor_mach;

    sol->makespan = 0;

    // Calculamos las operaciones iniciales de cada job:
    for (int i =0; i < sol->inst->num_jobs; i++) {
        job_op_current = &sol->inst->jobs[i][0];
        mach_op_current = &sol->machs[job_op_current->machine][0];

        if (mach_op_current->op == job_op_current) {
            availables[total_availables] = mach_op_current;
            total_availables++;
        }
    }

    while (total_availables > 0) {
        total_availables--;
        mach_op_current = availables[total_availables];
        
        job_op_current = mach_op_current->op;

        // Calculamos r_i
        predecesor_mach = get_predecesor_machine(sol, mach_op_current);
        predecesor_job = get_predecesor_job(sol, mach_op_current);

        int r_pred_mac = predecesor_mach != NULL ? predecesor_mach->r : 0;
        int r_pred_job = predecesor_job != NULL ? predecesor_job->r : 0;

        mach_op_current->r = max_int( 
            r_pred_mac + (predecesor_mach != NULL ? predecesor_mach->op->time : 0),  
            r_pred_job + ( predecesor_job != NULL ? predecesor_job->op->time : 0 )
        );

        sol->makespan = max_int( sol->makespan, mach_op_current->r + mach_op_current->op->time);

        sucesor_job = get_sucesor_job(sol, mach_op_current);

        // Verificamos si el siguiente trabajo en la secuencia del job puede ser planificado
        // para que el siguiente pueda ser planificad, el trabajo predecesor en la máquina debe estar planificado
        if ( 
            sucesor_job != NULL // existe el trabajo sucesor en el job
            && 
            ( !get_predecesor_machine(sol, sucesor_job) // no tiene trabajo predecesor en la máquina del sucesor
             || get_predecesor_machine(sol, sucesor_job)->r >= 0  // el predecesor en la maquina ya fue calendarizado
            )
        ) {
            availables[total_availables] = sucesor_job;
            total_availables++;
        }

        sucesor_mach = get_sucesor_machine(sol, mach_op_current);

        // Verificamos si el siguiente trabajo en la secuencia de la maquina puede ser planificado
        // para que el siguiente pueda ser planificado, el trabajo predecesor del job estar planificado
        if ( 
            sucesor_mach != NULL // existe el trabajo sucesor en la maquina
            &&
            (
                !get_predecesor_job(sol, sucesor_mach) // no tiene trabajo predecesor en la máquina del sucesor
                || get_predecesor_job(sol, sucesor_mach)->r >= 0 // el predecesor en el job ya fue calendarizado
            )
        ) {
            availables[total_availables] = sucesor_mach;
            total_availables++;
        }

    }
}

void calculate_length_tails(s_sol_perms_machs *sol) {

    s_op_schedule * availables[sol->inst->num_jobs + 1]; 
    int total_availables = 0;

    s_operacion *job_op_current;
    s_op_schedule * mach_op_current;

    s_op_schedule *predecesor_job;
    s_op_schedule * predecesor_mach;

    s_op_schedule *sucesor_job;
    s_op_schedule * sucesor_mach;

    sol->makespan = 0;

    // Calculamos las operaciones finales de cada job:
    for (int i =0; i < sol->inst->num_jobs; i++) {
        job_op_current = &sol->inst->jobs[i][sol->inst->num_machs - 1];
        mach_op_current = &sol->machs[job_op_current->machine][sol->inst->num_jobs - 1];

        if (mach_op_current->op == job_op_current) {
            availables[total_availables] = mach_op_current;
            total_availables++;
        }
    }

    while (total_availables > 0) {
        total_availables--;
        mach_op_current = availables[total_availables];
        
        job_op_current = mach_op_current->op;

        // Calculamos q_i
        sucesor_mach = get_sucesor_machine(sol, mach_op_current);
        sucesor_job = get_sucesor_job(sol, mach_op_current);

        int q_suc_mac = sucesor_mach != NULL ? sucesor_mach->q : 0;
        int q_suc_job = sucesor_job != NULL ? sucesor_job->q : 0;

        mach_op_current->t = max_int( q_suc_mac,  q_suc_job);
        mach_op_current->q = mach_op_current->t + mach_op_current->op->time;
        

        sol->makespan = max_int( sol->makespan, mach_op_current->q);

        predecesor_job = get_predecesor_job(sol, mach_op_current);

        // Verificamos si el predecesor en la secuencia del job puede ser calculado
        // para que el predecesor pueda ser calculado, el q del trabajo sucesor en la máquina debe estar calculado
        if ( 
            predecesor_job != NULL // existe el trabajo predecesor en el job
            && 
            ( !get_sucesor_machine(sol, predecesor_job) // no tiene sucesor en la máquina del predecesor
             || get_sucesor_machine(sol, predecesor_job)->q >= 0  // el sucesor en la maquina ya fue calculado
            )
        ) {
            availables[total_availables] = predecesor_job;
            total_availables++;
        }

        predecesor_mach = get_predecesor_machine(sol, mach_op_current);

        // Verificamos si el predecesor en la secuencia de la maquina puede ser calculado
        // para que el predecesor pueda ser planificado, el trabajo sucesor del job estar calculado
        if ( 
            predecesor_mach != NULL // existe el trabajo predecesor en la maquina
            &&
            (
                ! get_sucesor_job(sol, predecesor_mach) // no tiene trabajo sucesor en la máquina del precedesor
                || get_sucesor_job(sol, predecesor_mach)->q >= 0 // el sucesor en el job ya fue calendarizado
            )
        ) {
            availables[total_availables] = predecesor_mach;
            total_availables++;
        }

    }
}

void print_schedule(s_sol_perms_machs *sol) {
    for (int i =0; i < sol->inst->num_machs; i++) {
        printf("M %d [ ", i);
        for (int j=0; j < sol->inst->num_jobs; j++) { 
            printf("(J%d[%d] r=%d, d=%d, t=%d), ", 
                sol->machs[i][j].op->job , 
                sol->machs[i][j].op->seq, 
                sol->machs[i][j].r, 
                sol->machs[i][j].op->time,
                sol->machs[i][j].t
            );
        }
        printf("]\n");
    }
}