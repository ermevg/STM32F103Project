#ifndef LCD_H
#define LCD_H

#include "main.h"
#include "timer.h"

#define PIN_BLK      1
#define PIN_DC       2
#define PIN_RES      3
#define PIN_VCC      4
#define PIN_SCK      5
#define PIN_MOSI     7

#define LCD_BLACK   0x0000
#define LCD_WHITE   0xFFFF
#define LCD_RED     0xF800
#define LCD_GREEN   0x07E0
#define LCD_BLUE    0x001F
#define LCD_CYAN    0x07FF
#define LCD_MAGENTA 0xF81F
#define LCD_YELLOW  0xFFE0
#define LCD_ORANGE  0xFD20
#define LCD_GRAY    0x8410


void delay_ms(uint32_t delay);
void gpio_set(uint32_t pin);
void gpio_set(uint32_t pin);
void spi_lcd_transmit(uint8_t data);
void lcd_write_byte(uint8_t data);
void hardware_init(void);
void lcd_init(void);
void lcd_send_data(uint8_t cmd);
void lcd_send_cmd(uint8_t cmd);
void lcd_clear_full(uint16_t color);
void spi_wait_idle(void);

#endif