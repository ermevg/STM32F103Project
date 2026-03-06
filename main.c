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
    
    uint16_t start_check;

    uint8_t flag = 0;

    uint8_t flag2 = 0;

    uint16_t start_led2 = GET_TIM2_CNT();

    while(1)
    {
        
        if(check_time(&start) >= 30)
        {   
            
            start = GET_TIM2_CNT();
            if(GPIOA_READ_PIN0())
            {   flag = 1;
                GPIOB_SET_PIN2();
                start_check = GET_TIM2_CNT();
            }

        }

        if(flag)
        {
            pin_delay(&start_check, &flag);        
            
        }
        
        switch (flag2)
        {
            case 0:
                GPIOB_SET_PIN0();
                flag2 = 1;
                break;
            case 1:
                if(check_time(&start_led2) >= 1000)
                {
                    GPIOB_RESET_PIN0();
                    flag2 = 2;
                    start_led2 = GET_TIM2_CNT();
                }
                break;
            default:
                if(check_time(&start_led2) >= 1000)
                {
                    flag2 = 0;
                    start_led2 = GET_TIM2_CNT();
                }
                break;
        }
        //  GPIOB_SET_PIN0();
        //  for(volatile int i = 0; i < 1000000; i++);
        //  GPIOB_RESET_PIN0();
        //  for(volatile int i = 0; i < 1000000; i++);
    }

    return 0;
}