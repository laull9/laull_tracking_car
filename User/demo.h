#ifndef __DEMO_INCLUDE__
#define __DEMO_INCLUDE__

#include "laullstm32.h"
#include "arm_servo.h"
#include "wheel.h"
#include "camera.h"

typedef enum {
    choice_stop, choice_forward, choice_turnleft, choice_turnright, choice_other
}line_following_choice;

// 读取循迹模块的值
uint8_t read_tracking_sensor(uint16_t pin);

// 巡线控制函数
line_following_choice car_line_following_control();

// 初始化要使用的引脚
void demo_gpio_init();

// 初始化tim
void demo_tim_init();



#endif // !__DEMO_INCLUDE__
