#include "main.h"
#include "system_init.h"
#include "gpio.h"
#include "timer.h"


int main (void){

    SystemClock_Config();

    GPIOB_Init();
    
    GPIOA_Init();

    //TIM2_1ms_Init();


    while(1)
    {
        
    }

    return 0;
}