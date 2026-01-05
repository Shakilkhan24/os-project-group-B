// implementation of priority schedular 

#include "scheduler.h"
#include "process.h"

int currpid = -1;
static int time_quantum;
static int preempt;

// initialize the scheduler 
void sched_init(int quantum)
{
    time_quantum = quantum;
    preempt = quantum;
    currpid = -1;
}

// pick the next READY process with highest priority 
static int pick_next(void)
{
    int best = -1;

    for (int i = 0; i < MAX_PROCS; i++) {
        if (proctab[i].state == PR_READY) {
            if (best < 0 || proctab[i].priority > proctab[best].priority)
                best = i;
        }
    }
    return best;
}

// preempt based clk implementation
void tick(void)
{
    if (currpid < 0)
        return;

    preempt--;

    // preempt if higher-priority READY process exists
    for (int i = 0; i < MAX_PROCS; i++) {
        if (proctab[i].state == PR_READY &&
            proctab[i].priority > proctab[currpid].priority) {
            schedule();
            return;
        }
    }

    if (preempt <= 0) {
        preempt = time_quantum;
        schedule();
    }
}

// schedule next process 
void schedule(void)
{
    int next = pick_next();
    static void *old_sp = NULL;

    if (next < 0) {
        // No ready processes, do nothing 
        return;
    }

    // CURRENT → READY transtition 
    if (currpid >= 0 &&
        proctab[currpid].state == PR_CURRENT) {
        old_sp = proctab[currpid].stack_ptr;
        proctab[currpid].state = PR_READY;
    }

    currpid = next;
    proctab[currpid].state = PR_CURRENT;

    // context swithching between old and new process. 
    ctxsw(&old_sp, proctab[currpid].stack_ptr);
}