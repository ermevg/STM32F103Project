
#include "system_init.h"


#define RCC_BASE (0x40021000)
#define RCC_CR (*(volatile uint32_t*) (RCC_BASE + 0x00))
#define RCC_CFGR (*(volatile uint32_t*) (RCC_BASE + 0x04))
#define RCC_CIR (*(volatile uint32_t*) (RCC_BASE + 0x08))
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))

#define GPIOA_BASE (0x40010800)
#define GPIOA_CRL (*(volatile uint32_t*) (GPIOA_BASE + 0x00))
#define GPIOA_CRH (*(volatile uint32_t*) (GPIOA_BASE + 0x04))

#define GPIOA_APB2_CLOCK_EN (1 << 2)

#define PIN_MODE_RESET (~(0xF))
#define PA8_MODE (0xB << 0)
#define MCO_SRC_SYSCLK (4 << 24)


#define RCC_RESET (0)
#define RCC_HSI_ON (1 << 0)
#define RCC_HSE_ON (1 << 16)
#define RCC_HSE_RDY (1 << 17)

#define RCC_PLL_RDY (1 << 25)
#define RCC_PLL_ON (1 << 24)
#define RCC_PLL_SW (2 << 0)
#define PLL_MULL0 (0xF << 18)
#define PLL_MULL9 (0x7 << 18)

#define RCC_SW_RESET (~(0x03))
#define PLL_SRC (1 << 16)
#define RCC_SYSCLK_PLL (2 << 2)
#define RCC_CFGR_PPRE1_4 (0x5 << 8)
#define RCC_CFGR_PPRE1_RESET (0X7 << 8)


#define FLASH_ACR (*(volatile uint32_t*) 0x40022000)
#define FLASH_ACR_LATENCY0 (7 << 0) 
#define FLASH_ACR_LATENCY_2WS (0x2)


void SystemInit(void)
{
    RCC_CR |= RCC_HSI_ON;
    RCC_CFGR = RCC_RESET;
    RCC_CIR = RCC_RESET;
}

void SystemClock_Config_16MHZ(void)
{
    RCC_CR |= RCC_HSE_ON;
    while(!(RCC_CR & RCC_HSE_RDY));

    FLASH_ACR &= ~FLASH_ACR_LATENCY0;

    RCC_CFGR &= ~(PLL_MULL0 | PLL_SRC);
    RCC_CFGR |= PLL_SRC;

    RCC_CR |= RCC_PLL_ON;
    while(!(RCC_CR & RCC_PLL_RDY));

    RCC_CFGR &= RCC_SW_RESET;
    RCC_CFGR |= RCC_PLL_SW;

    while(!(RCC_CFGR & RCC_SYSCLK_PLL));
}

void SystemClock_Config_72MHZ(void)
{
    RCC_CR |= RCC_HSE_ON;
    while(!(RCC_CR & RCC_HSE_RDY));

    FLASH_ACR &= ~FLASH_ACR_LATENCY0;
    FLASH_ACR |= FLASH_ACR_LATENCY_2WS;

    RCC_CFGR &= ~(PLL_MULL0 | PLL_SRC | RCC_CFGR_PPRE1_RESET);
    RCC_CFGR |= PLL_MULL9 | PLL_SRC | RCC_CFGR_PPRE1_4;

    RCC_CR |= RCC_PLL_ON;
    while(!(RCC_CR & RCC_PLL_RDY));

    RCC_CFGR &= RCC_SW_RESET;
    RCC_CFGR |= RCC_PLL_SW;

    while(!(RCC_CFGR & RCC_SYSCLK_PLL));
}


void SYSCLK_CHECK(void)
{
    RCC_APB2ENR |= GPIOA_APB2_CLOCK_EN;
    GPIOA_CRH &= PIN_MODE_RESET;
    GPIOA_CRH |= PA8_MODE;
    RCC_CFGR &= ~(7 << 24);
    RCC_CFGR |= MCO_SRC_SYSCLK;
}