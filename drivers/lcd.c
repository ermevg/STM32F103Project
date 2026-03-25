#include "lcd.h"

#define RCC_BASE (0x40021000)
#define RCC_APB2ENR (*(volatile uint32_t*) (RCC_BASE + 0x18))

#define GPIOA_BASE (0x40010800)
#define GPIOA_CRL (*(volatile uint32_t*) (GPIOA_BASE + 0x00))
#define GPIOA_CRH (*(volatile uint32_t*) (GPIOA_BASE + 0x04))
#define GPIOA_BSRR (*(volatile uint32_t*) (GPIOA_BASE + 0x10))
#define GPIOA_BRR (*(volatile uint32_t*) (GPIOA_BASE + 0x14))
#define GPIOA_ODR (*(volatile uint32_t*) (GPIOA_BASE + 0x0C))
#define GPIOA_IDR (*(volatile uint32_t*) (GPIOA_BASE + 0x08))

#define GPIOA_APB2_CLOCK_EN (1 << 2)
#define PIN_MODE_RESET (0xF)
#define PA_MODE_OUTPUT_PUSH_PULL_50MHZ (0x3)
#define PA_MODE_OUTPUT_PUSH_PULL_50MHZ_AF (0xB)

#define SPI1_BASE (0x40013000)
#define SPI1_CR1 (*(volatile uint32_t*) (SPI1_BASE + 0x00))
#define SPI1_SR (*(volatile uint32_t*) (SPI1_BASE + 0x08))
#define SPI1_DR (*(volatile uint32_t*) (SPI1_BASE + 0x0C))

#define SPI1_APB2_CLOCK_EN (1 << 12)
#define SPI1_MODE_MSTR (1 << 2)
#define SPI1_MODE_BR (0x0 << 3)
#define SPI1_MODE_SSM (1 << 9)
#define SPI1_MODE_SSI (1 << 8)
#define SPI1_EN (1 << 6)
#define SPI1_BSY (1 << 7)
#define SPI1_TXE (1 << 1)
#define SPI1_RXNE (1 << 0)



// PA5 - SCK
// PA7 - MOSI

// Задержка
void delay_ms(uint32_t delay)
{
    uint16_t current_time = get_tim2_cnt();
    while((get_tim2_cnt() - current_time) < delay);
}

void gpio_set(uint32_t pin)
{
    GPIOA_BSRR = (1 << pin);
}

void gpio_reset(uint32_t pin)
{
    GPIOA_BRR = (1 << pin);
}

void spi_lcd_transmit(uint8_t data)
{
    while(!(SPI1_SR & SPI1_TXE));
    SPI1_DR = data;
    
}

// void lcd_write_byte(uint8_t data)
// {
//     spi_lcd_transmit(data);
// }

void spi_wait_idle(void) {
    while(SPI1_SR & SPI1_BSY);
}

void lcd_send_cmd(uint8_t cmd)
{   
    spi_wait_idle();
    gpio_reset(PIN_DC);
    spi_lcd_transmit(cmd);
}

void lcd_send_data(uint8_t cmd)
{   
    spi_wait_idle();
    gpio_set(PIN_DC);
    spi_lcd_transmit(cmd);
}

void hardware_init(void)
{
    RCC_APB2ENR |= GPIOA_APB2_CLOCK_EN | SPI1_APB2_CLOCK_EN;

    GPIOA_CRL &= ~(PIN_MODE_RESET << (PIN_BLK * 4));
    GPIOA_CRL &= ~(PIN_MODE_RESET << (PIN_RES * 4));
    GPIOA_CRL &= ~(PIN_MODE_RESET << (PIN_DC * 4));
    GPIOA_CRL &= ~(PIN_MODE_RESET << (PIN_VCC * 4));
    GPIOA_CRL &= ~(PIN_MODE_RESET << (PIN_SCK * 4));
    GPIOA_CRL &= ~(PIN_MODE_RESET << (PIN_MOSI * 4));

    GPIOA_CRL |= (PA_MODE_OUTPUT_PUSH_PULL_50MHZ << ((PIN_BLK) * 4));
    GPIOA_CRL |= (PA_MODE_OUTPUT_PUSH_PULL_50MHZ << ((PIN_RES) * 4));
    GPIOA_CRL |= (PA_MODE_OUTPUT_PUSH_PULL_50MHZ << ((PIN_DC) * 4));
    GPIOA_CRL |= (PA_MODE_OUTPUT_PUSH_PULL_50MHZ << ((PIN_VCC) * 4));
    GPIOA_CRL |= (PA_MODE_OUTPUT_PUSH_PULL_50MHZ_AF << ((PIN_SCK) * 4));
    GPIOA_CRL |= (PA_MODE_OUTPUT_PUSH_PULL_50MHZ_AF << ((PIN_MOSI) * 4));

    SPI1_CR1 = (SPI1_MODE_BR | SPI1_MODE_MSTR | SPI1_MODE_SSM | SPI1_MODE_SSI | (1 << 1) | (1 << 0));
    SPI1_CR1 |= SPI1_EN;

    
    gpio_set(PIN_BLK);
}

void lcd_init(void)
{
    hardware_init();

    gpio_set(PIN_RES);
    delay_ms(50);
    gpio_reset(PIN_RES);
    delay_ms(50);
    gpio_set(PIN_RES);
    delay_ms(150);

   
    lcd_send_cmd(0x01); // Software Reset
    delay_ms(150);
    lcd_send_cmd(0x11); // Sleep Out 
    delay_ms(120);

    lcd_send_cmd(0x3A); // Interface Pixel Format
    lcd_send_data(0x05); // 16-bit/pixel (RGB565)

    lcd_send_cmd(0x36); // Memory Access Control
    lcd_send_data(0x00); // Направление развертки

    lcd_send_cmd(0x21); // Display Inversion ON 
    lcd_send_cmd(0x13); // Normal Display Mode ON
    lcd_send_cmd(0x29); // Display ON 
}

void lcd_clear_full(uint16_t color) {
      // 1. Устанавливаем "окно" рисования
    lcd_send_cmd(0x2A); // Columns
    lcd_send_data(0x00); lcd_send_data(0x00); // Start 0
    lcd_send_data(0x00); lcd_send_data(0xEF); // End 239

    lcd_send_cmd(0x2B); // Rows
    lcd_send_data(0x00); lcd_send_data(0x00); // Start 0
    lcd_send_data(0x00); lcd_send_data(0xEF); // End 239

    // 2. Команда записи в память
    lcd_send_cmd(0x2C); 
    
    // Переключаемся в режим данных


    spi_wait_idle();
    gpio_set(PIN_DC);

    // 3. Цикл заливки
    for (uint32_t i = 0; i < (240 * 240); i++) {
        spi_lcd_transmit(color >> 8);   // Старший байт
        spi_lcd_transmit(color & 0xFF); // Младший байт
    }
}
