#ifndef TIMER_H
#define TIMER_H

#include "main.h"
#include "nvic_config.h"
#include "gpio.h"


void TIM2_1ms_Init(void);
uint32_t GET_TIM2_CNT(void);

#endif