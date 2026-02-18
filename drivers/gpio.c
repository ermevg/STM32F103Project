#include "gpio.h"

#define RCC_BASE (0x40021000)
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))

#define GPIOA_BASE (0x40010800)
#define GPIOA_CRL (*(volatile uint32_t*) (GPIOA_BASE + 0x00))
#define GPIOA_CRH (*(volatile uint32_t*) (GPIOA_BASE + 0x04))
#define GPIOA_BSRR (*(volatile uint32_t*) (GPIOA_BASE + 0x10))
#define GPIOA_ODR (*(volatile uint32_t*) (GPIOA_BASE + 0x0C))
#define GPIOA_IDR (*(volatile uint32_t*) (GPIOA_BASE + 0x08))

#define GPIOB_BASE (0x40010C00)
#define GPIOB_CRL (*(volatile uint32_t*) (GPIOB_BASE + 0x00))
#define GPIOB_BSRR (*(volatile uint32_t*) (GPIOB_BASE + 0x10))

#define GPIOA_APB2_CLOCK_EN (1 << 2)
#define GPIOB_APB2_CLOCK_EN (1 << 3)

#define PIN_MODE_RESET (~(0xF))

#define PA0_MODE_INPUT (~(3 << 0))
#define PA0_MODE_INPUT_PULLUP_PULLDOWN ((2 << 2))
#define PA0_PULL_DOWN (~(1 << 0))
#define PA0_ON (1 << 0)
#define PA0_OFF (1 << (0+16))
#define PA0_READ (1 << 0)



#define PB2_ON (1 << 2)
#define PB2_OFF (1 << (2+16))


void GPIOA_Init(void)
{
    RCC_APB2ENR |= GPIOA_APB2_CLOCK_EN;
    GPIOA_CRL &= PIN_MODE_RESET;
    GPIOA_CRL |= PA0_MODE_INPUT_PULLUP_PULLDOWN;
    GPIOA_ODR &= PA0_PULL_DOWN;
}

void GPIOB_Init(void)
{

    RCC_APB2ENR |= GPIOB_APB2_CLOCK_EN;
    GPIOB_CRL &= ~(0xF << 8);
    GPIOB_CRL |= (0x3 << 8);

}

void GPIOB_SET_PIN2(void)
{
    GPIOB_BSRR = PB2_ON;
}

void GPIOB_RESET_PIN2(void)
{
    GPIOB_BSRR = PB2_OFF;
}

uint32_t GPIOA_READ_PIN0(void)
{
    return (GPIOA_IDR & PA0_READ);
}