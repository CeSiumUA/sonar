#ifndef USART_H_
#define USART_H_

#include "stm32f4xx.h"
#include <unistd.h>

void initialize_usart_bt(uint32_t dst);
void usart_bt_write(uint32_t src, uint32_t dtlen);

#endif