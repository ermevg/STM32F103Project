#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include "main.h"

void SystemInit(void);
void SystemClock_Config_16MHZ(void);
void SystemClock_Config_72MHZ(void);
void SYSCLK_CHECK(void);

#endif