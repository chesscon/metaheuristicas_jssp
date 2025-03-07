#include <stdio.h>
#include <stdlib.h>
#include "permutations_machs_sol.h"
#include "evaluate_perms_machs_sol.h"
#include "repair_perms_sol.h"

void initial_job_operations(s_sol_perms_machs *sol, s_op_schedule **job_ops, int * total, int * labaleds) {
    *total = 0;

    s_op_schedule *current;

    for(int i =0; i < sol->inst->num_jobs; i++) {
        current = sol->ops[sol->inst->jobs[i][0].id];
        job_ops[i] = current;
        (*total)++;
        labaleds[current->op->id] += 1;
    }
}

void initial_mach_operations(s_sol_perms_machs *sol, s_op_schedule **mach_ops, int * total, int * labaleds) {
    *total = 0;

    s_op_schedule *current;

    for (int i=0; i < sol->inst->num_machs; i++) {
        current = &sol->machs[i][0];
        mach_ops[i] = current;
        (*total)++;
        labaleds[current->op->id] += 1;
    }   
}

void intersection_ops(
    s_op_schedule **mach_ops, int total_mach_ops,
    s_op_schedule **K, int * total_intersection,
    int * labaleds
) {
    *total_intersection = 0;

    s_op_schedule *current;

    for(int i =0; i < total_mach_ops; i++) {
        current = mach_ops[i];
        if (labaleds[current->op->id] == 2) {
            K[*total_intersection] = current;
            (*total_intersection)++;
        }
    }
}

void diff_ops(s_op_schedule **src_ops, int * total_src, s_op_schedule **diff_ops, int total_diff ) {
    for (int i =0; i < total_diff; i++) {
        
        // Buscamos el elemento en src_ops, y lo eliminamos
        for (int j=0; j < *total_src; j++) {
            if (diff_ops[i]->op->id == src_ops[j]->op->id) {

                // eliminamos el elementos (sobreescribimos con el que estaba en la ultima posicion)
                (*total_src)--;
                src_ops[j] = src_ops[*total_src];

                break; // rompemos el ciclo, ya encontramos el elemento
            }
        }

    }
}

void suc_job_operations(s_sol_perms_machs *sol, s_op_schedule **job_ops, int * total_job, s_op_schedule **K, int total_K, int * labaleds) {
    s_op_schedule *current;

    s_op_schedule *suc_job;
    for (int i =0; i < total_K; i++) {
        current = K[i];
        suc_job = get_sucesor_job(sol, current);
        if (suc_job != NULL) {
            job_ops[*total_job] = suc_job;
            (*total_job)++;
            labaleds[suc_job->op->id] += 1;
        }
    }
}

void suc_mach_operations(s_sol_perms_machs *sol, s_op_schedule **mach_ops, int * total_mach, s_op_schedule **K, int total_K, int * labaleds) {
    s_op_schedule *current;

    s_op_schedule *suc_mach;
    for (int i =0; i < total_K; i++) {
        current = K[i];
        suc_mach = get_sucesor_machine(sol, current);
        if (suc_mach != NULL) {
            mach_ops[*total_mach] = suc_mach;
            (*total_mach)++;
            labaleds[suc_mach->op->id] += 1;
        }
    }
}

void rearrenge_random_operation( 
    s_sol_perms_machs *sol, 
    s_op_schedule **job_ops, int total_job_ops, 
    s_op_schedule **mach_ops, int total_mach_ops, 
    s_op_schedule **K, int * total_insersection, 
    int * labaleds
) {
    
    int random_op = rand() % total_job_ops;
    s_op_schedule *current_job = job_ops[random_op];
    int mach = current_job->op->machine;

    printf("\n*** rearrange OP[%d], M%d. (J%d[%d], M%d[%d]) \n", 
        current_job->op->id,
        mach, 
        current_job->op->job,
        current_job->op->seq,
        current_job->op->machine,
        current_job->seq_m
    );

    s_op_schedule *current_mach = NULL;

    // Buscamos la operacion correspondiente a la maquina:
    int pos_mc =0;
    for (int i = 0; i < total_mach_ops; i++) {
        current_mach = mach_ops[i];
        if (mach == current_mach->op->machine) {
            pos_mc = i;
            i = total_mach_ops;
        }
    }

    printf("\n*** rearrange2 OP[%d], M%d. (J%d[%d], M%d[%d]) \n", 
        current_mach->op->id,
        mach, 
        current_mach->op->job,
        current_mach->op->seq,
        current_mach->op->machine,
        current_mach->seq_m
    );

    labaleds[current_mach->op->id]--;
    labaleds[current_job->op->id]++;

    int pos_a = current_job->seq_m;
    int pos_b = current_mach->seq_m;
    
    // intercambiamos las operacioens
    s_operacion *tmp = sol->machs[mach][pos_a].op;
    sol->machs[mach][pos_a].op = sol->machs[mach][pos_b].op;
    sol->ops[sol->machs[mach][pos_a].op->id] = &sol->machs[mach][pos_a];

    sol->machs[mach][pos_b].op = tmp;
    sol->ops[sol->machs[mach][pos_b].op->id] = &sol->machs[mach][pos_b];

    // Actualizamos los conjuntos
    mach_ops[pos_mc] = &sol->machs[mach][pos_b];
    job_ops[random_op] = &sol->machs[mach][pos_b];
    K[*total_insersection] = &sol->machs[mach][pos_b];
    (*total_insersection)++;


}

void print_ops(s_op_schedule **ops, int total_ops) {
    for (int i=0; i < total_ops; i++) {
        printf("OP[%d]=( J%d[%d], M%d[%d]) ", 
            ops[i]->op->id, 
            ops[i]->op->job,
            ops[i]->op->seq,
            ops[i]->op->machine,
            ops[i]->seq_m
        );
    }
    printf("\n");
}

void repair_solution(s_sol_perms_machs *sol) {
    s_op_schedule * job_ops[sol->inst->num_jobs];
    int total_job_ops = 0;
    s_op_schedule * machs_ops[sol->inst->num_machs];
    int total_mach_ops = 0;

    int total_ops =  sol->inst->num_jobs * sol->inst->num_machs;
    int labaleds[total_ops];
    for (int i =0; i < total_ops; i++) {
        labaleds[i] = 0;
    }

    // K : Intersection job operations \insercet mach operations
    s_op_schedule * K[sol->inst->num_machs];
    int total_insersection = 0;

    // PASO 1: 
    // -1.1 Calcular O_J (operaciones iniciales de cada job)
    initial_job_operations(sol, job_ops, &total_job_ops, labaleds);

    // -1.2 Calcular O_M (operaciones iniciales de cada job)
    initial_mach_operations(sol, machs_ops, &total_mach_ops, labaleds);

    // PASO 2: 
    // -2.1 Calcular K = O_J \interserc O_M
    intersection_ops( machs_ops, total_mach_ops, K, &total_insersection, labaleds);

    printf("O_J = ");
        print_ops(job_ops, total_job_ops);

        printf("O_M = ");
        print_ops(machs_ops, total_mach_ops);

        printf("K = ");
        print_ops(K, total_insersection);

    while (total_insersection > 0) {

        // -2.2 Actualizar O_J = O_J - K
        diff_ops(job_ops, &total_job_ops, K, total_insersection);

        // -2.3 Actualizar O_M = O_M - K
        diff_ops(machs_ops, &total_mach_ops, K, total_insersection);

        // - 2.4 Actualizar O_J y O_M (agreando sucesores)
        suc_job_operations(sol, job_ops, &total_job_ops, K, total_insersection, labaleds);
        suc_mach_operations(sol, machs_ops, &total_mach_ops, K, total_insersection, labaleds);

        intersection_ops( machs_ops, total_mach_ops, K, &total_insersection, labaleds);

        printf("\n **O_J = ");
        print_ops(job_ops, total_job_ops);

        printf("O_M = ");
        print_ops(machs_ops, total_mach_ops);

        printf("K = ");
        print_ops(K, total_insersection);

        if (total_insersection == 0 && ( total_job_ops > 0 || total_mach_ops > 0 )) {
            rearrenge_random_operation(sol, job_ops, total_job_ops, machs_ops, total_mach_ops, K, &total_insersection, labaleds);

            printf("\n *******O_J = ");
            print_ops(job_ops, total_job_ops);

            printf("\n *******O_M = ");
            print_ops(machs_ops, total_mach_ops);

            printf("\n *******K = ");
            print_ops(K, total_insersection);
        }
    }

    printf("O_J = ");
        print_ops(job_ops, total_job_ops);

        printf("O_M = ");
        print_ops(machs_ops, total_mach_ops);

        printf("K = ");
        print_ops(K, total_insersection);
    

}