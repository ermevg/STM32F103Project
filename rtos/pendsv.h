#ifndef PENDSV_H
#define PENDSV_H

#define SCB_BASE (0xE000ED00)
#define SCB_ICSR (*(volatile uint32_t*)(SCB_BASE + 0x04))
#define schedule() SCB_ICSR |= (1 << 28) 

#endif