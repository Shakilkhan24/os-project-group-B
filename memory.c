#include "memory.h"
extern uint8_t __kernel_end;

struct memblk memlist;
void *minheap;
void *maxheap;

void meminit(void)
{
    minheap = (void *)&__kernel_end;
    maxheap = (void *)0x01000000;  

    memlist.mnext = (struct memblk *)minheap;
    memlist.mlength = (uint32_t)(maxheap - minheap);

    memlist.mnext->mnext = NULL;
    memlist.mnext->mlength = memlist.mlength;
}


void* getmem(
    uint32_t nbytes  /* Size of memory requested */
) 
{
    struct memblk *prev, *curr, *leftover;

    if (nbytes == 0)
        return NULL;

    nbytes = roundmb(nbytes); /* Use memblk multiples */

    prev = &memlist;
    curr = memlist.mnext;

    while (curr != NULL) { /* Search free list */

        if (curr->mlength == nbytes) { /* Block is exact match */
            prev->mnext = curr->mnext;
            memlist.mlength -= nbytes;
            return (void *)curr;

        } else if (curr->mlength > nbytes) { /* split block */
            leftover = (struct memblk *)((uint8_t *)curr + nbytes);
            leftover->mlength = curr->mlength - nbytes;
            leftover->mnext = curr->mnext;

            prev->mnext = leftover;
            memlist.mlength -= nbytes;
            return (void *)curr;
        }

        prev = curr;    /* Move to next block */
        curr = curr->mnext;
    }

    return NULL;   
}


void freemem(void *blkaddr, uint32_t nbytes)
{
    struct memblk *block;
    if (blkaddr == NULL || nbytes == 0)
        return;

    nbytes = roundmb(nbytes);
    block = (struct memblk *)blkaddr;

    block->mlength = nbytes;
    block->mnext = memlist.mnext;
    memlist.mnext = block;

    memlist.mlength += nbytes;
}


void* getstk(uint32_t nbytes)
{
    return getmem(nbytes);
}


void freestk(void *ptr, uint32_t nbytes)
{
    freemem(ptr, nbytes);
}