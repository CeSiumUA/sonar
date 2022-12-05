#include "stm32f4xx.h"
#include "btncontrol.h"

int main(void){

    initialize_control_buttons();

    while(1){
    }
}

void EXTI9_5_IRQHandler(void){
    if(EXTI -> PR & EXTI_PR_PR5){
        process_left_button();

        EXTI -> PR |= EXTI_PR_PR5;
    }
    else if(EXTI -> PR & EXTI_PR_PR6){
        process_mode_selection();

        EXTI -> PR |= EXTI_PR_PR6;
    }
    else if(EXTI -> PR & EXTI_PR_PR8){
        process_right_button();

        EXTI -> PR |= EXTI_PR_PR8;
    }
}