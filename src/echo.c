#include "echo.h"

void initialize_echo_locators(void);
void initialize_left_locator(void);
void initialize_right_locator(void);
void configure_pins(void);

void initialize_echo_locators(void){

    TIM_DeInit(TIM3);

    RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;

    configure_pins();
    
    initialize_left_locator();
    initialize_right_locator();

    TIM3 -> PSC = 16 - 1;
    TIM3 -> CNT = 0;

    TIM3 -> CCR1 = 10;
    TIM3 -> CCR2 = 10;

    TIM3 -> ARR = 60000;

    TIM3 -> CR1 = TIM_CR1_CEN;
}

void initialize_left_locator(void){

    TIM3 -> CCMR1 &=~ TIM_CCMR1_CC1S_0;
    TIM3 -> CCMR1 &=~ TIM_CCMR1_CC1S_1;

    TIM3 -> CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
    TIM3 -> CCMR1 &=~TIM_CCMR1_OC1M_0;

    TIM3 -> CCER |= TIM_CCER_CC1E;
}

void initialize_right_locator(void){

    TIM3 -> CCMR1 &=~ TIM_CCMR1_CC2S_0;
    TIM3 -> CCMR1 &=~ TIM_CCMR1_CC2S_1;

    TIM3 -> CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
    TIM3 -> CCMR1 &=~TIM_CCMR1_OC2M_0;

    TIM3 -> CCER |= TIM_CCER_CC2E;
}

void configure_pins(void){
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA -> MODER &=~GPIO_MODER_MODER6_0;
    GPIOA -> MODER |= GPIO_MODER_MODER6_1;

    GPIOA -> MODER &=~GPIO_MODER_MODER7_0;
    GPIOA -> MODER |= GPIO_MODER_MODER7_1;

    GPIOA -> AFR[0] |= (1U<<25);
    GPIOA -> AFR[0] |= (1U<<29);
}