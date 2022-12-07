#ifndef SERVO_H_
#define SERVO_H_

#include "stm32f4xx.h"
#include <stdint.h>

void configure_pwm_pins(void);
void initialize_rotating_servo(void);
void set_vertical_rotation_angle(float angle);
void set_horizontal_rotation_angle(float angle);
float get_current_horizontal_angle(void);
float get_current_vertical_angle(void);
void rotate_vertical_inc(void);
void rotate_horizontal_inc(void);
void rotate_vertical_dec(void);
void rotate_horizontal_dec(void);

#endif