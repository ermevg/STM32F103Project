#ifndef KERNEL_INTERNAL_H
#define KERNEL_INTERNAL_H

#include "main.h"

extern uint32_t stack_space;
#define STACK_SIZE (128)

#define TASK_WAITING 0
#define TASK_READY 1
#define TASK_RUNNING 2

#define TASK_NAME_MAXLEN 16

struct task_block{
    char name[TASK_NAME_MAXLEN];
    int id;
    int state;
    void (*start)(void *arg);
    void *arg;
    uint32_t *sp;
};

struct stack_frame{
    uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
};

struct extra_frame{
    uint32_t r4, r5, r6, r7, r8, r9, r10, r11;
};

#define MAX_TASKS 8
#define kernel TASKS[0]

struct task_block *task_create (char* name, void (*start)(void 
*arg), void *arg);

void task_terminated(void);

#endif