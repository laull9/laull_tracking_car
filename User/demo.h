#ifndef __DEMO_INCLUDE__
#define __DEMO_INCLUDE__

#include "laullstm32.h"

// 轮子带电机
typedef struct{
    uint16_t pin_0, pin_1;
    GPIO_TypeDef* pin_mode_0, *pin_mode_1;
}wheel;

// 机械臂舵机
typedef struct{
    TIM_HandleTypeDef* htim;
    uint32_t tim_channel;
    uint16_t pin;
    GPIO_TypeDef* pin_mode;
}arm_servo;

// tim 计时器
extern TIM_HandleTypeDef tim2, tim3;

// 左前轮
extern wheel w1;
// 右前轮
extern wheel w2;
// 左后轮
extern wheel w3;
// 右后轮
extern wheel w4;

#define WHEEL_HTIM tim2
#define WHEEL_TIM_CHANNEL 3

// 巡线的黑白线检测反转
#define TRACKING_REVERSE 0
// 巡线模块的GPIO定义
#define TRACKING_PIN_1 GPIO_PIN_11
#define TRACKING_PIN_2 GPIO_PIN_12
#define TRACKING_PIN_3 GPIO_PIN_13
#define TRACKING_PIN_4 GPIO_PIN_14
#define TRACKING_PORT GPIOD




//设置TIM占空比
#define TIM_SET_DUTY_CYCLE(TIM_Handle, channel_num, duty_cycle_percent) do { \
    uint32_t period = (TIM_Handle).Instance->ARR;/* 获取当前周期 */ \
    uint32_t compare_value = (period * (duty_cycle_percent)) / 100U; /* 根据百分比计算比较值 */ \
    __HAL_TIM_SET_COMPARE(&TIM_Handle,TIM_CHANNEL_##channel_num, compare_value); /* 设置比较值 */ \
} while (0)


uint16_t angle_to_tim_val(float angle);
#define TIM_SET_ANGLE(TIM_Handle, channel_num, angle_num) \
    __HAL_TIM_SET_COMPARE(&TIM_Handle, TIM_CHANNEL_##channel_num, angle_to_tim_val(angle_num))

// 读取循迹模块的值
uint8_t read_tracking_sensor(uint16_t pin);

// 巡线控制函数
void car_line_following_control();

// 初始化要使用的引脚
void demo_gpio_init();

// 初始化tim
void demo_tim_init();



#endif // !__DEMO_INCLUDE__
