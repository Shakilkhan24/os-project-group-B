#include "scheduler.h"
#include "process.h"
#include "memory.h"

extern int currpid;
pcb_t proctab[MAX_PROCS];

// initialize the process table 
void proc_init(void)
{
    for (int i = 0; i < MAX_PROCS; i++) {
        proctab[i].pid = i;
        proctab[i].state = PR_TERMINATED;
        proctab[i].priority = 0;

        proctab[i].stack_base = NULL;
        proctab[i].stack_size = 0;
        proctab[i].stack_ptr  = NULL;
        proctab[i].entry = NULL;
    }
}

// create a new process
int create(void (*entry)(void), uint32_t stack_size, int priority)
{
    int pid;
    void *stack;
    uint32_t *sp;

    for (pid = 0; pid < MAX_PROCS; pid++) {
        if (proctab[pid].state == PR_TERMINATED)
            break;
    }

    if (pid == MAX_PROCS)
        return -1;

    stack = getstk(stack_size);
    if (!stack)
        return -1;

    proctab[pid].state = PR_READY;
    proctab[pid].priority = priority;
    proctab[pid].entry = entry;

    proctab[pid].stack_base = stack;
    proctab[pid].stack_size = stack_size;
    
    // Set up initial stack context 
    sp = (uint32_t *)((uint8_t *)stack + stack_size);
    
    // Push the process entry point on stack for ctxsw to return to
    *(--sp) = (uint32_t)entry;
    
    proctab[pid].stack_ptr = sp;

    return pid;
}

// terminate a process 
void kill(int pid)
{
    if (pid < 0 || pid >= MAX_PROCS)
        return;

    if (proctab[pid].state == PR_TERMINATED)
        return;

    freestk(proctab[pid].stack_base,
            proctab[pid].stack_size);

    proctab[pid].state = PR_TERMINATED;
    proctab[pid].priority = 0;
    proctab[pid].entry = NULL;
    proctab[pid].stack_base = NULL;
    proctab[pid].stack_size = 0;
    proctab[pid].stack_ptr  = NULL;
}

void exit(void)
{
    int pid = currpid;

    // change the state as terminated 
    proctab[pid].state = PR_TERMINATED;

    // free the stack memory 
    freestk(proctab[pid].stack_base,
            proctab[pid].stack_size);

    // meaning no current process right now 
    currpid = -1;

    // run next process 
    schedule();
}