#include "spi.h"

#define RCC_BASE (0x40021000)
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))

#define SPI1_BASE (0x40013000)
#define SP1_CR1 (*(volatile uint32_t*) (SPI1_BASE + 0x00))
#define SP1_SR (*(volatile uint32_t*) (SPI1_BASE + 0x08))
#define SP1_DR (*(volatile uint32_t*) (SPI1_BASE + 0x0C))


#define SPI1_APB2_CLOCK_EN (1 << 12)
#define SPI1_MODE_MSTR (1 << 2)
#define SPI1_MODE_BR (0x5 << 3)
#define SPI1_MODE_SSM (1 << 9)
#define SPI1_MODE_SSI (1 << 8)
#define SPI1_EN (1 << 6)
#define SPI1_BSY (1 << 7)
#define SPI1_TXE (1 << 1)
#define SPI1_RXNE (1 << 0)


#define GPIOA_BASE (0x40010800)
#define GPIOA_CRL (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_CRH (*(volatile uint32_t*)(GPIOA_BASE + 0x04))
#define GPIOA_BSRR (*(volatile uint32_t*)(GPIOA_BASE + 0x10))
#define GPIOA_ODR (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))
#define GPIOA_IDR (*(volatile uint32_t*)(GPIOA_BASE + 0x08))

#define GPIOA_APB2_CLOCK_EN (1 << 2)
#define GPIOA_PA4_MODE (0x3 << 16)
#define GPIOA_PA5_MODE (0xB << 20)
#define GPIOA_PA7_MODE (0xB << 28)
#define GPIO_PIN_RESET (0xF)


// PA5 - SCK
// PA7 - MOSI
// PA4 - NSS


void init_spi(void)
{
    RCC_APB2ENR |= GPIOA_APB2_CLOCK_EN | SPI1_APB2_CLOCK_EN;
    GPIOA_BSRR |= (1 << 4);
    GPIOA_CRL &= ~(GPIO_PIN_RESET << 16 | GPIO_PIN_RESET << 20 | GPIO_PIN_RESET << 28);
    GPIOA_CRL |= (GPIOA_PA4_MODE | GPIOA_PA5_MODE | GPIOA_PA7_MODE);
    SP1_CR1 = (SPI1_MODE_BR | SPI1_MODE_MSTR | SPI1_MODE_SSM | SPI1_MODE_SSI);
    SP1_CR1 |= SPI1_EN;
}

void spi_transmit(uint16_t data)
{
    GPIOA_BSRR |= (1 << (4 + 16));
    while (!(SP1_SR & SPI1_TXE));
    SP1_DR = (data >> 8) & 0xFF;
    while(!(SP1_SR & SPI1_RXNE));
    (void)SP1_DR;
    while (!(SP1_SR & SPI1_TXE));
    SP1_DR = data & 0xFF;
    while(!(SP1_SR & SPI1_RXNE));
    (void)SP1_DR;
    while (SP1_SR & SPI1_BSY);
    GPIOA_BSRR |= (1 << 4);
}