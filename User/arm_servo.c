#include "arm_servo.h"

void arm_servo_set_angle(arm_servo* as, float angle){
    __HAL_TIM_SET_COMPARE(as->htim, as->tim_channel, angle_to_tim_val(angle));
}


uint16_t angle_to_tim_val(float angle)
{
    uint16_t ret;
    if((angle < 0)||(angle > 180))
    {
        ret = 0;
    }
    else
    {
        ret = (1500 + (int)((float)((angle - 90) * 100 / 9)));
        if((ret < 500)||(ret > 2500))
        {
            ret = 0;
        }
    }
    return ret;
}


