#ifndef __ARM_SERVO_INCLUDE__
#define __ARM_SERVO_INCLUDE__

#include "laullstm32.h"
#include "setting.h"

uint16_t angle_to_tim_val(float angle);

void arm_servo_set_angle(arm_servo* as, float angle);

// 抓取物体（闭合夹持器）
void grab_object();

// 释放物体（张开夹持器）
void release_object();

void as2_up();

void as2_down();


#endif // !__ARM_SERVO_INCLUDE__