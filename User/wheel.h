#ifndef __WHEEL__INCLUDE__
#define __WHEEL__INCLUDE__

#include "laullstm32.h"
#include "setting.h"
#include "atk_ms6050.h"

void wheel_init(wheel* w);

void wheel_stop(wheel* w);

void wheel_run(wheel* w);

void wheel_runback(wheel* w);


//设置TIM占空比
// #define TIM_SET_DUTY_CYCLE(TIM_Handle, TIM_channel, duty_cycle_percent) do { \
//     uint32_t period = (TIM_Handle).Instance->ARR;/* 获取当前周期 */ \
//     uint32_t compare_value = (period * (duty_cycle_percent)) / 100U; /* 根据百分比计算比较值 */ \
//     __HAL_TIM_SET_COMPARE(&TIM_Handle, TIM_channel, compare_value); /* 设置比较值 */ \
// } while (0)

// 速度限幅宏
#define CLAMP_SPEED(speed) (speed) < 0 ? 0 : ((speed) > 100 ? 100 : (speed))

#define WHEEL_SET_NOW_SPEED(_wheel, _speedname) do { \
    uint32_t period = _wheel.speed_htim->Instance->ARR;/* 获取当前周期 */ \
    uint32_t compare_value = (period * (_wheel.speed._speedname)) / 100U; /* 根据百分比计算比较值 */ \
    __HAL_TIM_SET_COMPARE(_wheel.speed_htim, _wheel.speed_tim_channel, compare_value); /* 设置比较值 */ \
} while (0)

#define WHEEL_SET_SPEED_IMMIDIATELY(_speed, _wheel, _speedname)  do { \
    _wheel.speed._speedname = _speed; \
    uint32_t period = _wheel.speed_htim->Instance->ARR;/* 获取当前周期 */ \
    uint32_t compare_value = (period * (_speed)) / 100U; /* 根据百分比计算比较值 */ \
    __HAL_TIM_SET_COMPARE(_wheel.speed_htim, _wheel.speed_tim_channel, compare_value); /* 设置比较值 */ \
}while (0)

#define WHEEL_SET_NOW_SPEED_4(_w1, _w2, _w3, _w4, _speedname) do { \
    WHEEL_SET_NOW_SPEED(_w1, _speedname); \
    WHEEL_SET_NOW_SPEED(_w2, _speedname); \
    WHEEL_SET_NOW_SPEED(_w3, _speedname); \
    WHEEL_SET_NOW_SPEED(_w4, _speedname); \
} while (0)
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

void car_turn_right_2();

void car_turn_left_2();

// 陀螺仪控制
void tly_control(line_following_choice choice);


#endif // !__MONTER__INCLUDE__