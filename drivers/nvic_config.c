#include "nvic_config.h"

#define NVIC_BASE 0xE000E100
#define NVIC_ICER (NVIC_BASE + 0x080)
#define NVIC_ISER (NVIC_BASE + 0x00000000)


void nvic_irq_enable(uint8_t number)
{
    if(number > 81) return;
    uint8_t i = number/32;
    volatile uint32_t* nvic_en = (volatile uint32_t*)(NVIC_ISER)  + i;
    *nvic_en = (1 << (number % 32)); 
}

void nvic_irq_disable(uint8_t number)
{
    if(number > 81) return;
    uint8_t i = number/32;
    volatile uint32_t* nvic_en = (volatile uint32_t*)(NVIC_ICER) + i;
    *nvic_en = (1 << (number % 32)); 
}
