#ifndef __ARM_SERVO_INCLUDE__
#define __ARM_SERVO_INCLUDE__

#include "laullstm32.h"
#include "setting.h"

uint16_t angle_to_tim_val(float angle);

void arm_servo_set_angle(arm_servo* as, float angle);




#endif // !__ARM_SERVO_INCLUDE__