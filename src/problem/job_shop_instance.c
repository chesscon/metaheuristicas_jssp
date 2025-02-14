#include <stdio.h>
#include <stdlib.h>
#include "job_shop_instance.h"

s_jssp * allocate_instance(int num_jobs, int num_machs) {
    s_jssp *inst = NULL;
    inst = malloc( sizeof(s_jssp));
    inst->num_jobs = num_jobs;
    inst->num_machs = num_machs;
    inst->jobs = malloc( sizeof(s_operacion *)*num_jobs );

    for (int i=0; i < num_jobs; i++) {
        inst->jobs[i] = malloc(sizeof (s_operacion)*num_machs);
    }

    return inst;
}

void free_instance(s_jssp *instance) {
    for (int i=0; i < instance->num_jobs; i++) {
        free(instance->jobs[i]);
    }
    free(instance->jobs);
    free(instance);
}

s_jssp * make_instance_from_file(char *filename) {
    s_jssp *inst = NULL;
    FILE *in = fopen(filename, "r");
    
    int tmpScanf = 0;
    int num_jobs = 0;
    int num_machs = 0;

    tmpScanf = fscanf(in, "%d", &num_jobs);
    tmpScanf = fscanf(in, "%d", &num_machs);

    inst = allocate_instance(num_jobs, num_machs);

    int k = 0;
    for (int i = 0; i < num_jobs; i++) {
        for (int j=0; j < num_machs; j++) {
            inst->jobs[i][j].id = k++;
            inst->jobs[i][j].job = i;
            tmpScanf = fscanf(in, "%d", &inst->jobs[i][j].machine);
            inst->jobs[i][j].seq = j;
            tmpScanf = fscanf(in, "%d", &inst->jobs[i][j].time);
        }
    }
    fclose(in);

    return inst;
}

void print_instance(s_jssp *inst) {
    printf("#Jobs %d, #Machines: %d\n", inst->num_jobs, inst->num_machs);

    for(int i=0; i < inst->num_jobs; i++) {
        printf("J%d :", i+1);
        for (int j=0; j < inst->num_machs; j++) {
            printf(" (J%d[%d], M%d, %d), ", inst->jobs[i][j].job + 1, inst->jobs[i][j].seq + 1, inst->jobs[i][j].machine + 1, inst->jobs[i][j].time);
        }
        printf("\n");
    }
}
