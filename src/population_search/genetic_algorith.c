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
    return pob->inds[rand() % pob->size];
}

void mutate_sol(s_sol_perms_machs *sol, float pm) {
    int random = (rand()*1.0)/RAND_MAX;

    if (random <= pm) {
        // Necesitamos evaluar la solucion para poder calcular la vecindad
        eval_solution(sol);
        // generar veciondad
        s_neighborhood * neighborhood = allocate_neighborhood(sol->inst->num_jobs * sol->inst->num_machs);
        
        // elegir un vecino de manera aleatorio
        int rand_neighbor = rand() % neighborhood->total_neighbors;
        s_op_schedule *current = neighborhood->neighbors[rand_neighbor].a;
        int mach = current->op->machine;
        int pos_a = current->seq_m;
        int pos_b = neighborhood->neighbors[rand_neighbor].b->seq_m;

        // Aplicar movimiento:
        s_operacion *tmp = sol->machs[mach][pos_a].op;
        sol->machs[mach][pos_a].op = sol->machs[mach][pos_b].op;
        sol->ops[sol->machs[mach][pos_a].op->id] = &sol->machs[mach][pos_a];

        sol->machs[mach][pos_b].op = tmp;
        sol->ops[sol->machs[mach][pos_b].op->id] = &sol->machs[mach][pos_b];


        free_neighborhood(neighborhood);
    }
}

void mejorar_solucion(s_sol_perms_machs *sol) {
    s_sol_perms_machs *local_opt = local_search_jssp(sol);
    copy_sol_perms(local_opt, sol);
    free_sol_perms_machs(local_opt);
}

void genera_hijos(s_population *pob, float pc, float pm, s_sol_perms_machs *h1, s_sol_perms_machs *h2) {
    s_sol_perms_machs *p1 = NULL;
    s_sol_perms_machs *p2 = NULL;

    // Selecionar padres:
    p1 = select_parent(pob);
    p2 = select_parent(pob);

    int random = (rand()*1.0)/RAND_MAX;

    if (random <= pc) {
        hga_crossocer(p1, p2, h1, h2);   
    } else {
        copy_sol_perms(p1, h1);
        copy_sol_perms(p2, h2);
    }

    //  - Reparar soluciones
    repair_solution(h1);
    repair_solution(h2);

    //  - Aplicar mutación con probabilidad p1
    mutate_sol(h1, pm);
    mutate_sol(h2, pm);

    eval_solution(h1);
    eval_solution(h2);

    // Algoritmo memetico (quitar mutacion y agregar local search:)
    //mejorar_solucion(h1);
    //mejorar_solucion(h2);
    
}

void reemplaza_si_mejor_o_aleatorio(s_population *pob, s_sol_perms_machs *best, s_sol_perms_machs *h) {
    int idx_rand = 0;

    if (h->makespan < best->makespan) {
        copy_sol_perms(h, best);
    }
    idx_rand = rand() % pob->size;
    if (
        h->makespan < pob->inds[idx_rand]
        || rand()%2 == 0
    ) {
        copy_sol_perms(h, pob->inds[idx_rand]);
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

        eval_solution(h1);
        eval_solution(h2);

        if (h1->makespan < best->makespan) {
            copy_sol_perms(h1, best);
        }

        if (h2->makespan < best->makespan) {
            copy_sol_perms(h2, best);
        }
        
        update_population(pob, best, h1, h2);
    }

    free_population(pob);
    return best;
}
