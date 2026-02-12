
#include "system_init.h"

#define FLASH_ACR (*(volatile uint32_t*) 0x40022000)

#define RCC_BASE (0x40021000)
#define RCC_CR (*(volatile uint32_t*) (RCC_BASE + 0x00))
#define RCC_CFGR (*(volatile uint32_t*) (RCC_BASE + 0x04))
#define RCC_CIR (*(volatile uint32_t*) (RCC_BASE + 0x08))

#define RCC_RESET (0)
#define RCC_HSION (1<<0)

void SystemInit(void)
{
    RCC_CR |= RCC_HSION;
    RCC_CFGR = RCC_RESET;
    RCC_CIR = RCC_RESET;
}

void SystemClock_Config(void)
{

}