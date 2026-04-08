#include "kernel_internal.h"

struct task_block TASKS[MAX_TASKS];
int n_tasks = 1;
int running_task_id = 0;
static struct task_block *t_cur = &TASKS[0];
struct task_block *tasklist_active = NULL;
struct task_block *tasklist_waiting = NULL;


static void tasklist_add(struct task_block **list, struct task_block *el);
static int tasklist_del(struct task_block **list, struct task_block *delme);
static inline struct task_block *tasklist_next_ready(struct task_block *t);

void task_terminated(void)
{
    while(1);
}

static void task_stack_init(struct task_block *t)
{
    struct stack_frame *tf;
    t->sp -= sizeof(struct stack_frame)/4;
    tf = (struct stack_frame*)(t->sp);
    tf->r0 = (uint32_t)t->arg;
    tf->pc = (uint32_t)t->start;
    tf->lr = (uint32_t)task_terminated;
    tf->xpsr = (1 << 24);
    t->sp -= sizeof(struct extra_frame)/4;
};

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
    tasklist_add(&tasklist_active, t);
    return t;
    
}


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

void __attribute__((naked)) PendSV_Handler(void) 
{
    store_context();
    asm volatile("mrs %0, msp" : "=r"(t_cur->sp));
    if(t_cur->state == TASK_RUNNING){
        t_cur->state = TASK_READY;
    }
    t_cur = tasklist_next_ready(t_cur);
    t_cur->state = TASK_RUNNING;
    asm volatile ("msr msp, %0" :: "r"(t_cur->sp));
    restore_context();
    asm volatile("mov lr, %0" :: "r" (0xFFFFFFF9));
    asm volatile("bx lr");    
}


static void tasklist_add(struct task_block **list, struct task_block *el)
{
    
    el->next = *list;
    *list = el;

}

static int tasklist_del(struct task_block **list, struct task_block *delme)
{

    struct task_block *t = *list;
    struct task_block *p = NULL;
    while(t){
        if(t == delme){
            if(p == NULL)
                *list = t->next;
            else
                p->next = t->next;
            return 0;
        }
        p = t;
        t = t->next;
    }
    return -1;
}

static void task_waiting(struct task_block *t)
{
    if(tasklist_del(&tasklist_active, t) == 0){
        tasklist_add(&tasklist_waiting, t);
        t->state = TASK_WAITING;
    }

}

static void task_ready(struct task_block *t)
{
    if(tasklist_del(&tasklist_waiting, t) == 0){
        tasklist_add(&tasklist_active, t);
        t->state = TASK_READY;
    }

}

void sleep_ms(int ms)
{
    if(ms < TASK_TIMESLICE)
        return;
    t_cur->wakeup_time = jiffies + ms;
    task_waiting(t_cur);
    schedule();
}

static inline struct task_block *tasklist_next_ready(struct task_block *t)
{
    if((t->next == NULL) || (t->next->state != TASK_READY))
        return tasklist_active;
    return t->next;
}