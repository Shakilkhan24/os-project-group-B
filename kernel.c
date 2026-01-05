
#include "types.h"
#include "serial.h"
#include "string.h"
#include "memory.h"
#include "process.h"
#include "scheduler.h"
#define MAX_INPUT 128

void createcmd(const char *name);
void killcmd(char name);

struct {
    char name;
    int pid;
} pnmaeid[MAX_PROCS];

int pcount = 0;



void processA(void)
{
    for (int i = 0; i < 5; i++) {
        serial_puts("HI A\n");
        tick();
    }
    exit();
}

void processB(void)
{
    for (int i = 0; i < 5; i++) {
        serial_puts("HI B\n");
        tick();
    }
    exit();
}
void null_proc(void)
{
    serial_puts("Running null process...\n\n");
    char input[MAX_INPUT];
    int pos = 0;
    while (1) {
        serial_puts("kacchiOS> ");
        pos = 0;
        
        /* Read input line */
        while (1) {
            char c = serial_getc();
            
            /* Handle Enter key */
            if (c == '\r' || c == '\n') {
                input[pos] = '\0';
                serial_puts("\n");
                break;
            }
            /* Handle Backspace */
            else if ((c == '\b' || c == 0x7F) && pos > 0) {
                pos--;
                serial_puts("\b \b");  /* Erase character on screen */
            }
            /* Handle normal characters */
            else if (c >= 32 && c < 127 && pos < MAX_INPUT - 1) {
                input[pos++] = c;
                serial_putc(c);  /* Echo character */
            }
        }
        
        
        if (strcmp(input, "create(A)") == 0)
            createcmd("A");
        else if (strcmp(input, "create(B)") == 0)
            createcmd("B");
        else if (strcmp(input, "kill(A)") == 0)
            killcmd('A');
        else if (strcmp(input, "kill(B)") == 0)
            killcmd('B');
        else
            serial_puts("Unknown command\n");
        tick();
    }

}

void kmain(void) {
    
    
    /* Initialize hardware */
    serial_init();
    
    /* Print welcome message */
    serial_puts("\n");
    serial_puts("========================================\n");
    serial_puts("    kacchiOS - Minimal Baremetal OS\n");
    serial_puts("========================================\n");
    serial_puts("Hello from kacchiOS!\n");
    
    /* 👇 ADD ONLY THIS BLOCK */

    serial_puts("Kernel start\n");

    meminit();
    proc_init();

    sched_init(5);   /* configurable time quantum */
    /* Main loop - the "null process" */
    create(null_proc, 1024, 0);
    schedule();
    /* Should never reach here */
    for (;;) {
        __asm__ volatile ("hlt");
    }
}


void createcmd(const char *name)
{
    int pid = -1;

    if (strcmp(name, "A") == 0) {
        pid = create(processA, 1024, 10);
    }
    else if (strcmp(name, "B") == 0) {
        pid = create(processB, 1024, 5);
    }
    else {
        serial_puts("Unknown process\n");
        return;
    }

    if (pid >= 0) {
        pnmaeid[pcount].name = name[0];
        pnmaeid[pcount].pid = pid;
        pcount++;

        serial_puts("Process ");
        serial_putc(name[0]);
        serial_puts(" created\n");
    }
    else {
        serial_puts("Process creation failed\n");
    }
}
void killcmd(char name)
{
    for (int i = 0; i < pcount; i++) {
        if (pnmaeid[i].name == name) {
            kill(pnmaeid[i].pid);

            serial_puts("Process ");
            serial_putc(name);
            serial_puts(" killed\n");
            return;
        }
    }

    serial_puts("Process not found\n");
}