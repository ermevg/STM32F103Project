#include "uart_dma.h"

#define RCC_BASE    (0x40021000)
#define RCC_AHBENR  (*(volatile uint32_t*) (RCC_BASE + 0x14))
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))

#define GPIOA_APB2_CLOCK_EN (1 << 2)
#define USART1_APB2_CLOCK_EN (1 << 14)
#define DMA1_AHB_CLOCK_EN   (1 << 0)

#define USART1_BASE (0x40013800)
#define USART1_SR   (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define USART1_CR1  (*(volatile uint32_t*)(USART1_BASE + 0x0C))
#define USART1_CR3  (*(volatile uint32_t*)(USART1_BASE + 0x14))
#define USART1_BRR  (*(volatile uint32_t*)(USART1_BASE + 0x08))
#define USART1_DR   (*(volatile uint32_t*)(USART1_BASE + 0x04))

#define USART1_EN           (1 << 13)
#define USART1_TX_EN        (1 << 3)
#define USART1_RX_EN        (1 << 2)
#define USART1_BD           (0x8B) // 139
#define USART1_TXE          (1 << 7)
#define USART1_RXNE         (1 << 5)
#define USART1_CR3_DMAR     (1 << 6)
#define USART1_CR1_IDLEIE   (1 << 4)
#define USART1_IDLE         (1 << 4)

#define GPIOA_BASE  (0x40010800)
#define GPIOA_CRL   (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_CRH   (*(volatile uint32_t*)(GPIOA_BASE + 0x04))
#define GPIOA_BSRR  (*(volatile uint32_t*)(GPIOA_BASE + 0x10))
#define GPIOA_ODR   (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))
#define GPIOA_IDR   (*(volatile uint32_t*)(GPIOA_BASE + 0x08))

#define GPIOA_PA9_PA10_RESET (0xFF << 4)
#define GPIOA_SET_PA9_PA10_MODE (0x8B << 4)
#define GPIOA_PA10_PULL_UP  (1 << 10)

#define DMA1_BASE       (0x40020000)
#define DMA1_CCR5       (*(volatile uint32_t*) (DMA1_BASE + 0x58))
#define DMA1_CNDTR5     (*(volatile uint32_t*) (DMA1_BASE + 0x5C))
#define DMA1_CPAR5      (*(volatile uint32_t*) (DMA1_BASE + 0x60))
#define DMA1_CMAR5      (*(volatile uint32_t*) (DMA1_BASE + 0x64))

#define DMA_CCR5_EN     (1 << 0)
#define DMA_CCR5_MINC   (1 << 7)
#define DMA_CCR5_CIRC   (1 << 5)

#define USART1_IRQn     (37)

#define BUF_SIZE 64

uint8_t rx_buffer[BUF_SIZE];

void uart1_dma_rx_init(void){

    RCC_APB2ENR |= USART1_APB2_CLOCK_EN | GPIOA_APB2_CLOCK_EN;
    RCC_AHBENR |= DMA1_AHB_CLOCK_EN;
    GPIOA_CRH &= ~GPIOA_PA9_PA10_RESET;
    GPIOA_CRH |= GPIOA_SET_PA9_PA10_MODE;
    GPIOA_ODR |= GPIOA_PA10_PULL_UP;
    USART1_BRR = USART1_BD;
    USART1_CR1 |= USART1_EN | USART1_RX_EN | USART1_CR1_IDLEIE | USART1_TX_EN;
    USART1_CR3 |= USART1_CR3_DMAR;

    DMA1_CPAR5 = (uint32_t)&USART1_DR;
    DMA1_CMAR5 = (uint32_t)rx_buffer;
    DMA1_CNDTR5 = BUF_SIZE;
    DMA1_CCR5 |= DMA_CCR5_MINC | DMA_CCR5_CIRC | DMA_CCR5_EN;

    nvic_irq_enable(USART1_IRQn);

}

void USART1_IRQHandler(void)
{
    if(USART1_SR & USART1_IDLE)
    {
        (void)USART1_SR;
        (void)USART1_DR; 

        uint16_t data_lenght = BUF_SIZE - DMA1_CNDTR5;

    }
}