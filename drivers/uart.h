#ifndef UART_H
#define UART_H

#include "main.h"

void init_uart1(void);

void uart1_send_stirng(char* buffer);

void uart1_send_char(char c);

void uart1_send_int(int number);


#endif