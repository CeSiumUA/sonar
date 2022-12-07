#include "stm32f4xx.h"
#include "btncontrol.h"
#include "servo.h"
#include "usart.h"

char message[13] = "hello world\r\n";

void process_control_buttons(void);

void init_irqs(void){
    __disable_irq();
    __enable_irq();
}

int main(void){

    RCC_DeInit();

    init_irqs();

    initialize_control_buttons();

    initialize_rotating_servo();

    initialize_usart_bt((uint32_t)message);

    usart_bt_write((uint32_t)message, 13);

    while(1){
        process_control_buttons();
    }
}

void process_control_buttons(void){
    if(GPIOC -> IDR & GPIO_IDR_IDR_5){
        set_left_button(1);
    }
    else{
        set_left_button(0);
    }

    if(GPIOC -> IDR & GPIO_IDR_IDR_8){
        set_right_button(1);
    }
    else{
        set_right_button(0);
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

void DMA1_Stream5_IRQHandler(void){
    DMA1 -> HIFCR |= (DMA_HIFCR_CFEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTCIF5);
}

void DMA1_Stream6_IRQHandler(void){
    DMA1 -> HIFCR |= (DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6);
}

void EXTI9_5_IRQHandler(void){
    if(EXTI -> PR & EXTI_PR_PR6){
        EXTI -> PR |= EXTI_PR_PR6;
        process_mode_selection();
    }
}