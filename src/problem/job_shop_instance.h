#ifndef JOB_SHOP_INSTANCE_H
#define JOB_SHOP_INSTANCE_H

typedef struct {
    int id;
    int job;
    int machine;
    int time;
    int seq;
} s_operacion;

typedef struct {
    int num_jobs;
    int num_machs;
    
    s_operacion **jobs;
} s_jssp;

s_jssp * allocate_instance(int num_jobs, int num_machs);
void free_instance(s_jssp *instance);

s_jssp * make_instance_from_file(char *filename);

void print_instance(s_jssp *inst);

#endif