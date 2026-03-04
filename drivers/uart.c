#include "uart.h"

#define RCC_BASE (0x40021000)
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))

#define USART1_BASE (0x40013800)
#define USART1_SR (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define USART1_CR1 (*(volatile uint32_t*)(USART1_BASE + 0x0C))
#define USART1_BRR (*(volatile uint32_t*)(USART1_BASE + 0x08))
#define USART1_DR (*(volatile uint32_t*)(USART1_BASE + 0x04))
#define GPIOA_BASE (0x40010800)
#define GPIOA_CRL (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_CRH (*(volatile uint32_t*)(GPIOA_BASE + 0x04))
#define GPIOA_BSRR (*(volatile uint32_t*)(GPIOA_BASE + 0x10))
#define GPIOA_ODR (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))
#define GPIOA_IDR (*(volatile uint32_t*)(GPIOA_BASE + 0x08))

#define GPIOA_APB2_CLOCK_EN (1 << 2)
#define UART1_APB2_CLOCK_EN (1 << 14)
#define GPIOA_PA9_PA10_RESET (0xFF << 4)
#define GPIOA_SET_PA9_PA10_MODE (0x8B << 4)
#define GPIOA_PA10_PULL_UP (1 << 10)
#define USART1_EN (1 << 13)
#define USART1_TX_EN (1 << 3)
#define USART1_RX_EN (1 << 2)
#define USART1_BD (0x8B) // 139
#define USART1_TXE (1 << 7)

void init_uart1(void)
{
    RCC_APB2ENR |= UART1_APB2_CLOCK_EN;
    RCC_APB2ENR |= GPIOA_APB2_CLOCK_EN;
    GPIOA_CRH &= ~GPIOA_PA9_PA10_RESET;
    GPIOA_CRH |= GPIOA_SET_PA9_PA10_MODE;
    GPIOA_ODR |= GPIOA_PA10_PULL_UP;
    USART1_BRR = USART1_BD;
    USART1_CR1 |= USART1_TX_EN;
    USART1_CR1 |= USART1_RX_EN;
    USART1_CR1 |= USART1_EN;
}

void uart1_send_char(char* buffer)
{
    while(*buffer != 0)
    {
        while(!(USART1_SR & USART1_TXE));
        USART1_DR = *buffer;
        buffer++;
    }    
    
    
}