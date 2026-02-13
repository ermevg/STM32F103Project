#include "gpio.h"

#define RCC_BASE (0x40021000)
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))

#define GPIOB_BASE (0x40010C00)
#define GPIOB_CRL (*(volatile uint32_t*) (GPIOB_BASE + 0x00))
#define GPIOB_BSSR (*(volatile uint32_t*) (GPIOB_BASE + 0x10))

#define GPIOB_APB2_CLOCK_EN (1 << 3)

void GPIO_Init(void)
{
    RCC_APB2ENR |= GPIOB_APB2_CLOCK_EN;
    GPIOB_CRL &= ~(0xF << 8);
    GPIOB_CRL |= (0x3 << 8);
    GPIOB_BSSR = (1 << (2));
    
    uint32_t i = 0;

    while (i < 1000000){ i++ ;}
    
    GPIOB_BSSR = (1 << (2+16));
    
    

};