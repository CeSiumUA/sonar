#ifndef BTNCONTROL_H_
#define BTNCONTROL_H_

#include "stm32f4xx.h"

void initialize_control_buttons(void);
void process_left_button(void);
void process_right_button(void);
void process_mode_selection(void);
int get_left_button_state(void);
int get_right_button_state(void);
int get_middle_button_state(void);

#endif