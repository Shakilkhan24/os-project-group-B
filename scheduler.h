#ifndef SCHEDULER_H
#define SCHEDULER_H

void sched_init(int quantum);
void schedule(void);
void tick(void);

// context switch 
void ctxsw(void **old_sp, void *new_sp);

#endif