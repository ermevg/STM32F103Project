#ifndef UART_H
#define UART_H

#include "main.h"
#include "nvic_config.h"
#include "lcd.h"

void init_uart1(void);

void uart1_send_string(char* buffer);

void uart1_send_char(char c);

void uart1_send_int(int number);

uint8_t uart1_recieve(void);

#endif
