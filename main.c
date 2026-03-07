#include "main.h"
#include "system_init.h"
#include "gpio.h"
#include "timer.h"
#include "nvic_config.h"
#include "uart.h"
#include "spi.h"
#include "exti.h"



int main (void){

    SystemClock_Config();

    init_spi();

    init_exti0();
    
    while(1)
    {
       
    }

    return 0;
}
