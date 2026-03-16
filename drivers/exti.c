#include "exti.h"
#include "adc.h"
#include "uart.h"


#define AFIO_BASE (0x40010000)
#define AFIO_EXTICR1 (*(volatile uint32_t*)(AFIO_BASE + 0x08))

#define AFIO_APB2_CLOCK_EN (1 << 0)
#define EXTI0_PA0 (~(0xF))

#define GPIOA_BASE (0x40010800)
#define GPIOA_CRL (*(volatile uint32_t*) (GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile uint32_t*) (GPIOA_BASE + 0x0C))

#define GPIOA_APB2_CLOCK_EN (1 << 2)
#define GPIOA0_RESET_MODE (~(0xF))
#define PA0_MODE_INPUT_PULLUP_PULLDOWN ((2 << 2))
#define PA0_PULL_DOWN (~(1 << 0))

#define RCC_BASE (0x40021000)
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))

#define EXTI_BASE (0x40010400)
#define EXTI_IMR (*(volatile uint32_t*) (EXTI_BASE + 0x00)) 
#define EXTI_RTSR (*(volatile uint32_t*) (EXTI_BASE + 0x08)) 
#define EXTI_FTSR (*(volatile uint32_t*) (EXTI_BASE + 0x0C)) 
#define EXTI_PR (*(volatile uint32_t*) (EXTI_BASE + 0x14)) 


#define EXTI0_IRQ_RISING_EDGE (1 << 0)
#define EXTI0_IRQ_EN (1 << 0)
#define EXTI0_IRQ_OCCUR (1 << 0)
#define EXTI0_IRQn 6

void init_exti0(void)
{
    RCC_APB2ENR |= AFIO_APB2_CLOCK_EN | GPIOA_APB2_CLOCK_EN;
    GPIOA_CRL &= GPIOA0_RESET_MODE;
    GPIOA_CRL |= PA0_MODE_INPUT_PULLUP_PULLDOWN;
    GPIOA_ODR &= PA0_PULL_DOWN;
    AFIO_EXTICR1 &= EXTI0_PA0;
    EXTI_IMR |= EXTI0_IRQ_EN;
    EXTI_RTSR |= EXTI0_IRQ_RISING_EDGE;
    EXTI_FTSR &= ~EXTI0_IRQ_EN;
    nvic_irq_enable(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_PR & EXTI0_IRQ_OCCUR)
    {
        // for(volatile int i = 0; i < 200000; i++); 
        // spi_transmit(read_adc());
        //uart1_send_char("PING!\r\n");
        uart1_send_int(-1369);
        EXTI_PR |= EXTI0_IRQ_OCCUR;
    }
}