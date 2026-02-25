#include "main.h"
#include "system_init.h"
#include "gpio.h"
#include "timer.h"
#include "tasks.h"




int main (void){
 
    SystemClock_Config();
    
    GPIOB_Init();
    
    GPIOA_Init();
    
    TIM2_1MS_INIT();    


    uint16_t start = GET_TIM2_CNT();
    
    while(1)
    {
        if(check_time(&start) >= 30)
        {   

            if(GPIOA_READ_PIN0())
            {
                pin_delay();
                start = GET_TIM2_CNT();
            }
        }

    }

    return 0;
}