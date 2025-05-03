#ifndef __DEMO_INCLUDE__
#define __DEMO_INCLUDE__

#include "laullstm32.h"
#include "arm_servo.h"
#include "wheel.h"
#include "camera.h"
#include "usart_commad.h"
#include "atk_ms6050.h"

typedef enum {
    choice_stop, choice_forward, choice_backward, choice_turnleft, choice_turnright, choice_moveleft, choice_moveright, choice_other
}line_following_choice;

// 速度限幅宏
#define CLAMP_SPEED(speed) \
    do { \
        if (speed < 0) speed = 0; \
        else if (speed > 100) speed = 100; \
    } while(0)

// 读取循迹模块的值
uint8_t read_tracking_sensor(uint16_t pin);

// 巡线控制函数
line_following_choice car_line_following_control();

// 初始化要使用的引脚
void demo_gpio_init();

// 初始化tim
void demo_tim_init();

// 陀螺仪控制
void tly_control(line_following_choice choice);



#endif // !__DEMO_INCLUDE__
