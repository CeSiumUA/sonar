#ifndef BTNCONTROL_H_
#define BTNCONTROL_H_

#include "stm32f4xx.h"

void initialize_control_buttons(void);
void process_left_button(void);
void process_right_button(void);
void process_mode_selection(void);

#endif