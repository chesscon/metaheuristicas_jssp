#include <stdio.h>
#include <stdlib.h>
#include "tabu_search.h"
#include "../neighborhood/N1.h"

s_tabu_list * allocate_tabu_list( int max_size ) {
    s_tabu_list *tabu_list;

    tabu_list = malloc(sizeof(s_tabu_list));
    tabu_list->list = malloc(sizeof(s_move)*max_size);
    tabu_list->size = 0;
    tabu_list->max_size = max_size;

    return tabu_list;
}

void free_tabu_list(s_tabu_list * tabu_list) {
    free(tabu_list->list);
    free(tabu_list);
}

int is_move_allowed( 
    s_move *move, 
    s_tabu_list *tabu_list,
    int current_iteration) { 

    for (int i = 0; i < tabu_list->size; i++) {

        if (
            tabu_list->list[i].mach == move->a->op->machine 
            &&
            tabu_list->list[i].job_a == move->a->op->job 
            &&
            tabu_list->list[i].job_b == move->b->op->job 
        ) {
            return 0;
        };
    }

    return 1;
}

int select_index_neighbor(int index_neighbor_best, s_neighborhood * neighborhood, int current_index ) {
    if (index_neighbor_best < 0) {
        return current_index;
    }

    if (neighborhood->neighbors[current_index].eval < neighborhood->neighbors[index_neighbor_best].eval) {
        return current_index;
    }

    int random = rand() % 2;

    if (random == 0) {
        return current_index;
    }

    return index_neighbor_best;
}

int check_aspiration_criteria(s_move *move, s_sol_perms_machs *best) {
    if (move->eval >= best->makespan) {
        return 0;
    } 
    s_sol_perms_machs *clone = clone_sol_perms(best);
    move->eval = apply_and_evaluate_move_N1(clone, move);

    free_sol_perms_machs(clone);
    
    if (move->eval < best->makespan) {
        return 1;
    }

    return 0;
}

int get_index_neighbor_best_not_tabu(  
    s_sol_perms_machs * sol,
    s_neighborhood * neighborhood,
    s_sol_perms_machs *best,
    s_tabu_list *tabu_list,
    int current_iteration

) {
    int index_neighbor_best = -1;

    for (int i =0; i < neighborhood->total_neighbors; i++) {
        neighborhood->neighbors[i].eval = estimate_evaluation_neighbor(sol, &neighborhood->neighbors[i]);

        if ( 
            ( 
                check_aspiration_criteria(&neighborhood->neighbors[i], best) // cumple el criterio de aspiracion
                || is_move_allowed(&neighborhood->neighbors[i], tabu_list, current_iteration ) // no es tabu
            )
         && (
            index_neighbor_best == -1 
            || neighborhood->neighbors[i].eval <= neighborhood->neighbors[index_neighbor_best].eval  
            ) 
        ) {
            index_neighbor_best = select_index_neighbor(index_neighbor_best, neighborhood, i);
        }
        
    }

    if (index_neighbor_best == -1) {
        return rand() % neighborhood->total_neighbors;
    }

    return index_neighbor_best;
    
}

void update_tabu_list(s_tabu_list *tabu_list, s_move move, int current_iteration, int min_tabu_tenure) {
    s_tabu_move tmp;

    // Eliminamos los movimientos que ya "caduaron"
    for (int i =0; i < tabu_list->size; i++) {
        if ( tabu_list->list[i].last_tabu_it < current_iteration) {
            // el movimiento en la lista ya no esta prohibido y hay que borrarlo:
            tmp = tabu_list->list[i];
            tabu_list->size--;
            tabu_list->list[i] = tabu_list->list[tabu_list->size];
            tabu_list->list[tabu_list->size] = tmp;
            i--;
            continue;
        }
    }

    int pos;

    if (tabu_list->size < tabu_list->max_size) {
        pos = tabu_list->size;
        tabu_list->size++;
    } else {
        // Si ya esta llena la lista, elegimos un movimiento al azar
        pos = rand() % tabu_list->max_size;
    }
    
    int tabu_tenure = min_tabu_tenure + (rand() % ( tabu_list->max_size - min_tabu_tenure ));

    tabu_list->list[pos].mach = move.a->op->machine;
    tabu_list->list[pos].job_a = move.b->op->job;
    tabu_list->list[pos].job_b = move.a->op->job;
    tabu_list->list[pos].last_tabu_it = current_iteration + tabu_tenure;

    /*
    printf("\t Agregando el mov a tabu[%d] hasta %d, (M%d[%d], (J%d, J%d)) \n", 
        pos,
        tabu_list->list[pos].last_tabu_it,
        move.a->op->machine,
        move.a->seq_m,
        move.a->op->job,
        move.b->op->job
    );
    */
    
}

s_sol_perms_machs * tabu_search_jssp(
    s_sol_perms_machs * initial_sol, 
    int max_iters,
    int max_size_tabu,
    int min_tabu_tenure
) {
    s_sol_perms_machs *best = clone_sol_perms(initial_sol);
    s_sol_perms_machs *current = clone_sol_perms(initial_sol);

    s_neighborhood * neighborhood = allocate_neighborhood(current->inst->num_jobs * current->inst->num_machs);
    s_tabu_list *tabu_list = allocate_tabu_list(max_size_tabu);
    int best_index;
    int iteration = 0;

    while (iteration < max_iters) {
        iteration++;
    
        generate_neighbors_n1(current, neighborhood);
        best_index = get_index_neighbor_best_not_tabu(current, neighborhood, best, tabu_list, iteration);
        apply_and_evaluate_move_N1(current, &neighborhood->neighbors[best_index]);

        // Actualizar la lista tabu
        update_tabu_list(tabu_list, neighborhood->neighbors[best_index], iteration, min_tabu_tenure);

        printf("Iter: %d, best: %d, estimate_best_neighbor: %d, exact_best_neighbor: %d, (M%d[%d], (J%d, J%d)) \n", 
            iteration, best->makespan, neighborhood->neighbors[best_index].eval, current->makespan,
            neighborhood->neighbors[best_index].a->op->machine,
            neighborhood->neighbors[best_index].a->seq_m,
            neighborhood->neighbors[best_index].a->op->job,
            neighborhood->neighbors[best_index].b->op->job
        );

        if (current->makespan < best->makespan) {
            copy_sol_perms(current, best);
        }
    };

    free_tabu_list(tabu_list);
    free_neighborhood(neighborhood);
    free_sol_perms_machs(current);

    return best;
}