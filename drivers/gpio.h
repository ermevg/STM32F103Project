#ifndef GPIO_H
#define GPIO_H

#include "main.h"

void GPIOA_Init(void);

void GPIOB_Init(void);

void GPIOB_SET_PIN2(void);

void GPIOB_RESET_PIN2(void);

void GPIOB_SET_PIN0(void);

void GPIOB_RESET_PIN0(void);

uint32_t GPIOA_READ_PIN0(void);

#endif
