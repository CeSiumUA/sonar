#ifndef SERVO_H_
#define SERVO_H_

#include "stm32f4xx.h"
#include <stdint.h>

const int MAX_ANGLE_PULSE = 2500;
const int MIN_ANGLE_PULSE = 500;
const float DEGREES_IN_USEC = 0.09f;

void configure_pwm_pins(void);
void initialize_rotating_servo(void);
void set_vertial_rotation_angle(float angle);
void set_horizontal_rotation_angle(float angle);
float get_current_horizontal_angle(void);
float get_current_vertical_angle(void);

#endif