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

    GPIOC -> PUPDR |= GPIO_PUPDR_PUPDR5_1;
    GPIOC -> PUPDR &=~GPIO_PUPDR_PUPDR5_0;

    GPIOC -> PUPDR |= GPIO_PUPDR_PUPDR6_1;
    GPIOC -> PUPDR &=~GPIO_PUPDR_PUPDR6_0;

    GPIOC -> PUPDR |= GPIO_PUPDR_PUPDR8_1;
    GPIOC -> PUPDR &=~GPIO_PUPDR_PUPDR8_0;

    SYSCFG -> EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PC;

    EXTI -> IMR |= EXTI_IMR_MR6;

    EXTI -> RTSR |= EXTI_RTSR_TR6;

    NVIC_EnableIRQ(EXTI9_5_IRQn);
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

void set_left_button(int state){
    is_left_button_pressed = state;
}

void set_right_button(int state){
    is_right_button_pressed = state;
}

void set_mode_selection(int state){
    is_mode_vertical = state;
}

int get_left_button_state(void){
    return is_left_button_pressed;
}

int get_right_button_state(void){
    return is_right_button_pressed;
}

int get_middle_button_state(void){
    return is_mode_vertical;
}