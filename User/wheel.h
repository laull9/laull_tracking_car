#ifndef __WHEEL__INCLUDE__
#define __WHEEL__INCLUDE__

#include "laullstm32.h"
#include "demo.h"

void wheel_init(wheel w);

void wheel_stop(wheel w);

void wheel_run(wheel w);

void wheel_runback(wheel w);

#define wheel_setspeed(speed) TIM_SET_DUTY_CYCLE(WHEEL_HTIM, WHEEL_TIM_CHANNEL, speed)

// 直走
void car_go_forward();
// 倒车
void car_go_backward();
// 左转
void car_turn_left();
// 右转
void car_turn_right();
// 平左移动
void car_move_left();
// 平右移动
void car_move_right();
// 停止
void car_stop();


#endif // !__MONTER__INCLUDE__