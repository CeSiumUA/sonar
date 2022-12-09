#include "stm32f4xx.h"
#include "btncontrol.h"
#include "servo.h"
#include "usart.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define COMMAND_BUFFER_LENGTH 4

#define USE_BLUETOOTH

char command_buffer[COMMAND_BUFFER_LENGTH];
char last_receive_command[COMMAND_BUFFER_LENGTH];

void process_control_buttons(void);

void init_irqs(void){
    __disable_irq();
    __enable_irq();
}

int main(void){

    RCC_DeInit();

    init_irqs();

    #ifdef USE_BUTTONS
    initialize_control_buttons();
    #endif

    #ifdef USE_SERVO
    initialize_rotating_servo();
    #endif

    #ifdef USE_BLUETOOTH
    initialize_usart_bt((uint32_t)command_buffer, COMMAND_BUFFER_LENGTH);
    #endif

    while(1){
        #ifdef USE_BUTTONS
        process_control_buttons();
        #endif
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
}

void DMA2_Stream2_IRQHandler(void){
    DMA2 -> HIFCR |= (DMA_LIFCR_CFEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CHTIF2 | DMA_LIFCR_CTCIF2);

    bool is_command_new = memcmp(command_buffer, last_receive_command, COMMAND_BUFFER_LENGTH);
    if(is_command_new){
        usart_bt_write((uint32_t)command_buffer, COMMAND_BUFFER_LENGTH);
        memmove(last_receive_command, command_buffer, COMMAND_BUFFER_LENGTH);
    }
    //int mode = command_buffer[0] - '0';
    //float angle = atoff(command_buffer + 1);
    #ifdef USE_SERVO
    if(mode){
        set_horizontal_rotation_angle(angle);
    }
    else{
        set_vertical_rotation_angle(angle);
    }
    #endif
}

void DMA2_Stream6_IRQHandler(void){
    DMA2 -> HIFCR |= (DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6);

    DMA2_Stream6 -> CR &=~DMA_SxCR_EN;
}

void EXTI9_5_IRQHandler(void){
    #ifdef USE_SERVO
    if(EXTI -> PR & EXTI_PR_PR6){
        EXTI -> PR |= EXTI_PR_PR6;
        process_mode_selection();
    }
    #endif
}