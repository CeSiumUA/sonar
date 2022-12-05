#include "stm32f4xx.h"
#include "btncontrol.h"
#include "servo.h"

void init_irqs(void){
    __disable_irq();
    __enable_irq();
}

int main(void){

    init_irqs();

    initialize_control_buttons();

    initialize_rotating_servo();

    while(1){
    }
}

void TIM2_IRQHandler(void){
    TIM2 -> SR &=~TIM_SR_UIF;

    int mode = get_middle_button_state();

    if(get_left_button_state()){
        if(mode){
            rotate_vertical_inc();
        }
        else{
            rotate_horizontal_inc();
        }
    }
    else if(get_right_button_state()){
        if(mode){
            rotate_vertical_dec();
        }
        else{
            rotate_horizontal_dec();
        }
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