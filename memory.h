#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define roundmb(x)   ((uint32_t)(((x) + 7) & ~7))
#define truncmb(x)   ((uint32_t)((x) & ~7))

struct memblk {
    struct memblk *mnext;   // Ptr to next free memory blk 
    uint32_t mlength;       // Size of blk (includes memblk) 
};

extern struct memblk memlist;    // Head of free memory list 
extern void *minheap;           // Start of heap 
extern void *maxheap;            // Highest valid heap address 

void meminit(void);
void* getmem(uint32_t nbytes);
void freemem(void *ptr, uint32_t nbytes);
void* getstk(uint32_t nbytes);
void freestk(void *ptr, uint32_t nbytes);

#endif