#ifndef __USART_COMMAND__
#define __USART_COMMAND__

#include "setting.h"
#include "wheel.h"
#include "arm_servo.h"

extern bool commands_looping;

// 处理usart控制信号
void parse_commands(uint8_t* buffer, uint16_t len);

void run_commanda_loop();

#endif