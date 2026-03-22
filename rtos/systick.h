#ifndef SYSTICK_H
#define SYSTICK_H

#include "main.h"
#include "pendsv.h"
#define TIMESLICE (20)

extern volatile unsigned long jiffies;

void systick_init(void);

#endif