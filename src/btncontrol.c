#include "btncontrol.h"

static int is_left_button_pressed = 0;
static int is_right_button_pressed = 0;
static int is_mode_vertical = 0;

void initialize_control_buttons(void){
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    GPIOC -> MODER &=~GPIO_MODER_MODER5_0;
    GPIOC -> MODER &=~GPIO_MODER_MODER5_1;

    GPIOC -> MODER &=~GPIO_MODER_MODER6_0;
    GPIOC -> MODER &=~GPIO_MODER_MODER6_1;

    GPIOC -> MODER &=~GPIO_MODER_MODER8_0;
    GPIOC -> MODER &=~GPIO_MODER_MODER8_1;

    SYSCFG -> EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PC;
    SYSCFG -> EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PC;
    SYSCFG -> EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC;

    EXTI -> IMR |= EXTI_IMR_MR5;
    EXTI -> IMR |= EXTI_IMR_MR6;
    EXTI -> IMR |= EXTI_IMR_MR8;

    EXTI -> RTSR |= EXTI_RTSR_TR5;
    EXTI -> RTSR |= EXTI_RTSR_TR6;
    EXTI -> RTSR |= EXTI_RTSR_TR8;

    EXTI -> FTSR |= EXTI_FTSR_TR5;
    EXTI -> FTSR |= EXTI_FTSR_TR6;
    EXTI -> FTSR |= EXTI_FTSR_TR8;

    NVIC_EnableIRQ(EXTI9_5_IRQn);

    __disable_irq();

    __enable_irq();
}

void process_left_button(void){
    is_left_button_pressed = !is_left_button_pressed;
}

void process_right_button(void){
    is_right_button_pressed = !is_right_button_pressed;
}

void process_mode_selection(void){
    is_mode_vertical = !is_mode_vertical;
}