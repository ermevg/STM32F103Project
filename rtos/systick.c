#include "systick.h"

#define STK_BASE (0xE000E010)
#define STK_CTRL (*(volatile uint32_t*)(STK_BASE + 0x00))
#define STK_LOAD (*(volatile uint32_t*)(STK_BASE + 0x04))
#define STK_VAL (*(volatile uint32_t*)(STK_BASE + 0x08))

volatile unsigned long jiffies = 0;

void systick_init(void) {

    STK_CTRL = 0;
    STK_LOAD = 15999;
    STK_VAL = 0;
    STK_CTRL = 0x07;
    
}

void SysTick_Handler(void)
{
    if((++jiffies % TIMESLICE) == 0)
    schedule();
}