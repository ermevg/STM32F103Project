#include "main.h"
#include "system_init.h"
#include "gpio.h"
#include "timer.h"
// #include "nvic_config.h"
//#include "uart.h"
#include "spi.h"
// #include "exti.h"
// #include "adc.h"
#include "uart_dma.h"

//extern uint8_t rx_buffer[];

int main (void){

    SystemClock_Config();

    TIM2_1ms_Init();

    init_spi();

    while(1)
    {
        
        
    }

    return 0;
}
