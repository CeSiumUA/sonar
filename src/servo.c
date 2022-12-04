#include "servo.h"

void configure_pwm_pins(void);
void initialize_rotating_servo(void);
void set_vertial_rotation_angle(float angle);
void set_horizontal_rotation_angle(float angle);
float get_current_horizontal_angle(void);
float get_current_vertical_angle(void);

float get_current_vertical_angle(void){
    uint32_t usecs = TIM2 -> CCR1;

    return usecs * DEGREES_IN_USEC;
}

float get_current_horizontal_angle(void){
    uint32_t usecs = TIM2 -> CCR2;

    return usecs * DEGREES_IN_USEC;
}

int get_time_angle(float angle){
    float usecs = angle / DEGREES_IN_USEC;

    usecs += MIN_ANGLE_PULSE;

    if(usecs > MAX_ANGLE_PULSE){
        usecs = MAX_ANGLE_PULSE;
    }

    return (int)usecs;
}

void set_vertical_rotation_angle(float angle){
    int usecs = get_time_angle(angle);

    TIM2 -> CCR1 = usecs;
}

void set_horizontal_rotation_angle(float angle){
    int usecs = get_time_angle(angle);
    
    TIM2 -> CCR2 = usecs;
}

void initialize_rotating_servo(void){
    TIM_DeInit(TIM2);

    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;

    configure_pwm_pins();

    TIM2 -> CCMR1 &=~ TIM_CCMR1_CC1S_0;
    TIM2 -> CCMR1 &=~ TIM_CCMR1_CC1S_1;

    TIM2 -> CCMR1 &=~ TIM_CCMR1_CC2S_0;
    TIM2 -> CCMR1 &=~ TIM_CCMR1_CC2S_1;

    TIM2 -> CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
    TIM2 -> CCMR1 &=~TIM_CCMR1_OC1M_0;

    TIM2 -> CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
    TIM2 -> CCMR1 &=~TIM_CCMR1_OC2M_0;

    TIM2 -> CCER |= TIM_CCER_CC1E;
    TIM2 -> CCER |= TIM_CCER_CC2E;

    TIM2 -> PSC = 16 - 1;
    TIM2 -> CNT = 0;

    TIM2 -> CCR1 = 0;
    TIM2 -> CCR2 = 0;
    TIM2 -> ARR = 20000 - 1;

    TIM2 -> CR1 = TIM_CR1_CEN;
}

void configure_pwm_pins(void){
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA -> MODER &=~GPIO_MODER_MODER5_0;
    GPIOA -> MODER |= GPIO_MODER_MODER5_1;

    GPIOA -> MODER &=~GPIO_MODER_MODER1_0;
    GPIOA -> MODER |= GPIO_MODER_MODER1_1;

    GPIOA -> AFR[0] |= (1U<<20);
    GPIOA -> AFR[0] |= (1U<<4);
}