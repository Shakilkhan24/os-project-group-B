#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

// total processes 
#define MAX_PROCS 8

// process states 
typedef enum {
    PR_CURRENT,
    PR_READY,
    PR_TERMINATED
} proc_state_t;

// Process block 
typedef struct pcb {
    int pid;
    proc_state_t state;

    int priority;            

    void *stack_base;
    uint32_t stack_size;
    void *stack_ptr;

    void (*entry)(void);   // process function 
} pcb_t;

// global process table 
extern pcb_t proctab[MAX_PROCS];

// functions used in process creation 
void proc_init(void);
int create(void (*entry)(void), uint32_t stack_size, int priority);
void kill(int pid);
void exit(void);

#endif