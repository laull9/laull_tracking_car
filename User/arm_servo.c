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


// 抓取物体（闭合夹持器）
void grab_object() {
    arm_servo_set_angle(&as5, GRAB_ANGLE);
    // delay_ms(500);
}

// 释放物体（张开夹持器）
void release_object() {
    arm_servo_set_angle(&as5, RELEASE_ANGLE);
    // delay_ms(500);
}

// 机械臂左转（基座旋转）
void rotate_base_left() {
    arm_servo_set_angle(&as1, BASE_LEFT_ANGLE);
    // delay_ms(1000);
}

// 机械臂右转（基座旋转）
void rotate_base_right() {
    arm_servo_set_angle(&as1, BASE_RIGHT_ANGLE);
    // delay_ms(1000);
}

// 机械臂收起（折叠姿态）
void retract_arm() {
    // 先收起远端关节
    arm_servo_set_angle(&as4, RETRACT_AS4);
    delay_ms(300);
    arm_servo_set_angle(&as3, RETRACT_AS3);
    delay_ms(300);
    arm_servo_set_angle(&as2, RETRACT_AS2);
    delay_ms(300);
}