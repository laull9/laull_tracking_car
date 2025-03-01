#ifndef __WHEEL__INCLUDE__
#define __WHEEL__INCLUDE__

#include "laullstm32.h"
#include "setting.h"

void wheel_init(wheel* w);

void wheel_stop(wheel* w);

void wheel_run(wheel* w);

void wheel_runback(wheel* w);


//设置TIM占空比
#define TIM_SET_DUTY_CYCLE(TIM_Handle, TIM_channel, duty_cycle_percent) do { \
    uint32_t period = (TIM_Handle).Instance->ARR;/* 获取当前周期 */ \
    uint32_t compare_value = (period * (duty_cycle_percent)) / 100U; /* 根据百分比计算比较值 */ \
    __HAL_TIM_SET_COMPARE(&TIM_Handle, TIM_channel, compare_value); /* 设置比较值 */ \
} while (0)

#define car_set_speed(speed) TIM_SET_DUTY_CYCLE(WHEEL_HTIM, WHEEL_TIM_CHANNEL, speed)

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