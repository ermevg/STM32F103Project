#include "timer.h"
#include "nvic_config.h"
#include "gpio.h"


#define RCC_BASE (0x40021000)
#define RCC_APB1ENR (*(volatile uint32_t*) (RCC_BASE + 0x1C))

#define RCC_TIM2_EN (1 << 0)

#define TIM2_BASE (0x40000000)
#define TIM2_CR1 (*(volatile uint32_t*) (TIM2_BASE + 0x00))
#define TIM2_PSC (*(volatile uint32_t*) (TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile uint32_t*) (TIM2_BASE + 0x2C))
#define TIM2_EGR (*(volatile uint32_t*) (TIM2_BASE + 0x14))
#define TIM2_SR (*(volatile uint32_t*) (TIM2_BASE + 0x10))
#define TIM2_CNT (*(volatile uint32_t*) (TIM2_BASE + 0x24))
#define TIM2_DIER (*(volatile uint32_t*) (TIM2_BASE + 0x0C))

#define TIM2_EGR_UPDATE (1 << 0)
#define TIM2_CEN (1 << 0)
#define TIM2_ARR_CNT (1000)
#define TIM2_IRQ_EN (1 << 0)
#define TIM2_UIF (1 << 0)

#define TIM2_IRQn 28


void TIM2_1ms_Init(void)
{
    RCC_APB1ENR |= RCC_TIM2_EN;
    TIM2_PSC = 15999;
    TIM2_ARR = TIM2_ARR_CNT;
    TIM2_EGR |= TIM2_EGR_UPDATE;
    TIM2_SR &= ~(TIM2_EGR_UPDATE);
    nvic_irq_enable(TIM2_IRQn);
    TIM2_DIER |= TIM2_IRQ_EN;
    TIM2_CR1 |= TIM2_CEN;
}

uint32_t GET_TIM2_CNT(void)
{
    uint32_t cnt = TIM2_CNT;
    return cnt;
}

void TIM2_IRQHandler(void)
{
    static uint8_t ticks = 0;

    if(TIM2_SR & TIM2_UIF) TIM2_SR &= ~TIM2_UIF; 
    if (ticks == 0)
    {
        GPIOB_SET_PIN2();
        ticks = 1;
    }
    else
    {
        GPIOB_RESET_PIN2();
        ticks = 0;
    }
    
    
}
