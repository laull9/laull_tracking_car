#include "arm_servo.h"

void arm_servo_set_angle(arm_servo* as, float angle){
    __HAL_TIM_SET_COMPARE(as->htim, as->tim_channel, angle_to_tim_val(angle));
    as->now_angle = angle;
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


void grab_object() {
    arm_servo_set_angle(&as1, GRAB_ANGLE);
    delay_ms(300);
}


void release_object() {
    arm_servo_set_angle(&as1, RELEASE_ANGLE);
    delay_ms(300);
}


void as2_up() {
    u8 step = 4;
    if (as2.now_angle < RETRACT_AS2){
        for (float now = as2.now_angle; now <= RETRACT_AS2; now += step){
            arm_servo_set_angle(&as2, now);
            delay_ms(50);
        }
    }
    else{
        for (float now = as2.now_angle; now >= RETRACT_AS2; now -= step){
            arm_servo_set_angle(&as2, now);
            delay_ms(50);
        }
    }
}

void as2_down() {
    u8 step = 4;
    if (as2.now_angle < RELEASE_AS2){
        for (float now = as2.now_angle; now <= RELEASE_AS2; now += step){
            arm_servo_set_angle(&as2, now);
            delay_ms(50);
        }
    }
    else{
        for (float now = as2.now_angle; now >= RELEASE_AS2; now -= step){
            arm_servo_set_angle(&as2, now);
            delay_ms(50);
        }
    }
}