#include <stdio.h>
#include <stdlib.h>
#include "genetic_algorithm.h"
#include "../solution/permutations_machs_sol.h"
#include "../solution/evaluate_perms_machs_sol.h"
#include "../population/population.h"
#include "../recombination/hga_crossover.h"
#include "../solution/repair_perms_sol.h"
#include "../neighborhood/N1.h"
#include "../trajectory_search/local_search.h"

s_sol_perms_machs * select_parent(s_population *pob) {
    int random_pos = rand() % pob->size;
    //printf("Selected : %d \n", random_pos);
    return pob->inds[random_pos];
}

void mutate_sol(s_sol_perms_machs *sol, float pm) {
    double random = ((double)rand())/(double)RAND_MAX;
    printf("random mutation: %.6f, pm = %.6f \n", random, pm);

    if (random <= pm) {
        s_neighborhood * neighborhood = allocate_neighborhood(sol->inst->num_jobs * sol->inst->num_machs);
        // Necesitamos evaluar la solucion para poder calcular la vecindad
        eval_solution(sol);
        printf("> PREV EVAL: %d \n", sol->makespan);
        // generar veciondad
        generate_neighbors_n1(sol, neighborhood);
        
        // elegir un vecino de manera aleatorio
        int rand_neighbor = rand() % neighborhood->total_neighbors;
        

        // Aplicar movimiento:

        printf("\t **** --> MOV: (J%d[%d], M%d[%d]) <---> (J%d[%d], M%d[%d]) \n", 
            neighborhood->neighbors[rand_neighbor].a->op->job,
            neighborhood->neighbors[rand_neighbor].a->op->seq,
            neighborhood->neighbors[rand_neighbor].a->op->machine,
            neighborhood->neighbors[rand_neighbor].a->seq_m,

            neighborhood->neighbors[rand_neighbor].b->op->job,
            neighborhood->neighbors[rand_neighbor].b->op->seq,
            neighborhood->neighbors[rand_neighbor].b->op->machine,
            neighborhood->neighbors[rand_neighbor].b->seq_m
        );

        printf("\n******SOLP PREV: \n");
        print_schedule(sol);

        // Aplicar movimiento:
        apply_move_N1(sol, &neighborhood->neighbors[rand_neighbor]);
        repair_solution(sol);
        eval_solution(sol);

        printf("> NEW EVAL: %d \n", sol->makespan);

        printf("\n ******NEW SOL: \n");
        print_schedule(sol);
        

        free_neighborhood(neighborhood);
    }
}

void mejorar_solucion(s_sol_perms_machs *sol) {
    s_sol_perms_machs *local_opt = local_search_jssp(sol);
    copy_sol_perms(local_opt, sol);
    free_sol_perms_machs(local_opt);
}

void genera_hijos(s_population *pob, float pc, float pm, s_sol_perms_machs *h1, s_sol_perms_machs *h2) {
    // Selecionar padres:
    s_sol_perms_machs *p1 = select_parent(pob);
    s_sol_perms_machs *p2 = select_parent(pob);

    double random = ((double)rand())/(double)RAND_MAX;
    
    /*
    printf("Crossover operation ***** \n");
    printf("random cross: %.6f \n", random);
    printf("pc: %.6f \n", pc);
    printf("compare: %d \n", random <= pc);
    */

    if (random <= pc) {
//        printf("Crossover parents...\n");
        hga_crossover(p1, p2, h1, h2);   
    } else {
//        printf("clone parents...\n");
        copy_sol_perms(p1, h1);
        copy_sol_perms(p2, h2);

    }

    //  - Reparar soluciones
    //printf("repair solutions...\n");
    repair_solution(h1);
    repair_solution(h2);

    /* 
    printf("\n Solución Reparada h1 \n");
    print_sol_perms_machs(h1);

    printf("\n Solución Reparada h2 \n");
    print_sol_perms_machs(h2);
    */

    //  - Aplicar mutación con probabilidad p1
    mutate_sol(h1, pm);
    mutate_sol(h2, pm);

//    printf("evaluate solutions...\n");
    eval_solution(h1);
    printf("MAKESPAN H1: %d ...\n", h1->makespan);
    eval_solution(h2);
    printf("MAKESPAN H2: %d ...\n", h2->makespan);

    // Algoritmo memetico (quitar mutacion y agregar local search:)
    mejorar_solucion(h1);
    mejorar_solucion(h2);   
}

void reemplaza_si_mejor_o_aleatorio(s_population *pob, s_sol_perms_machs *best, s_sol_perms_machs *h) {

    if (h->makespan < best->makespan) {
        printf("\n \t ****** BEST *********************************** \t \n");
        copy_sol_perms(h, best);
    }

    int idx_rand = rand() % pob->size;
    if (
        h->makespan < pob->inds[idx_rand]->makespan
        || rand()%2 == 0
    ) {
        copy_sol_perms(pob->inds[idx_rand], h);
    }   
}

void update_population(s_population *pob, s_sol_perms_machs *best, s_sol_perms_machs *h1, s_sol_perms_machs *h2) {
    reemplaza_si_mejor_o_aleatorio(pob, best, h1);
    reemplaza_si_mejor_o_aleatorio(pob, best, h2);
}

s_sol_perms_machs * genetic_algorithm_jssp(
    s_jssp *inst, 
    int size, 
    int max_iters,
    float pc,
    float pm
) {

    s_population *pob = make_random_population(inst, size);
    int idx_best = evaluate_population(pob);

    s_sol_perms_machs *best = clone_sol_perms(pob->inds[idx_best]);

    s_sol_perms_machs *h1 = allocate_sol_perms(inst);
    s_sol_perms_machs *h2 = allocate_sol_perms(inst);

    int it = 0;
    while (it < max_iters ) {
        it++;
        
        genera_hijos(pob, pc,pm, h1, h2);
        
        update_population(pob, best, h1, h2);
    }

    free_sol_perms_machs(h1);
    free_sol_perms_machs(h2);
    free_population(pob);
    return best;
}
