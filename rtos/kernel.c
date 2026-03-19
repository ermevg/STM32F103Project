#include "kernel_internal.h"

static struct task_block TASKS[MAX_TASKS];
static int n_tasks = 1;
static int running_task_id = 0;

void task_terminated(void)
{
    while(1);
}

struct task_block *task_create (char* name, void (*start)(void 
*arg), void *arg)
{
    struct task_block *t;
    int i;
    if(n_tasks >= MAX_TASKS)
        return NULL;
    t = &TASKS[n_tasks];
    t->id = n_tasks++;
    for(i = 0; i < TASK_NAME_MAXLEN; i++){
        t->name[i] = name[i];
        if(name[i] == 0)
            break;
    }
    t->state = TASK_READY;
    t->start = start;
    t->arg = arg;
    t->sp = ((&stack_space) + n_tasks * STACK_SIZE);
    task_stack_init(t);
    return t;
    
}

static void task_stack_init(struct task_block *t)
{
    struct stack_frame *tf;
    t->sp -= sizeof(struct stack_frame);
    tf = (struct stack_frame*)(t->sp);
    tf->r0 = (uint32_t)t->arg;
    tf->pc = (uint32_t)t->start;
    tf->lr = (uint32_t)task_terminated;
    tf->xpsr = (1 << 24);
    t->sp -= sizeof(struct extra_frame);
};

static void __attribute__((naked)) store_context(void)
{
    asm volatile("mrs r0, msp");
    asm volatile("stmdb r0!, {r4-r11}");
    asm volatile("msr msp, r0");
    asm volatile("bx lr");
}

static void __attribute__((naked)) restore_context(void)
{
    asm volatile("mrs r0, msp");
    asm volatile("ldmfd r0!, {r4-r11}");
    asm volatile("msr msp, r0");
    asm volatile("bx lr");
}

