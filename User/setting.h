#ifndef __SETTING_INCLUDE__
#define __SETTING_INCLUDE__
#include "laullstm32.h"

// 轮子速度
typedef struct{
    uint8_t forward, backward, moveleft, moveright, turnleft, turnright;
}wheel_speed;

// 轮子带电机
typedef struct{
    uint16_t pin_0, pin_1;
    GPIO_TypeDef* pin_mode_0, *pin_mode_1;
    TIM_HandleTypeDef* speed_htim;
    uint32_t speed_tim_channel;
    wheel_speed speed;
}wheel;

// 机械臂舵机
typedef struct{
    TIM_HandleTypeDef* htim;
    uint32_t tim_channel;
    float now_angle;
}arm_servo;

// tim 计时器
extern TIM_HandleTypeDef tim2, tim3, tim4;


// 左前轮
extern wheel w1;
// 右前轮
extern wheel w2;
// 左后轮
extern wheel w3;
// 右后轮
extern wheel w4;

#define WHEEL_PIN_1_1 GPIO_PIN_1
#define WHEEL_PIN_1_2 GPIO_PIN_2
#define WHEEL_PIN_2_1 GPIO_PIN_3
#define WHEEL_PIN_2_2 GPIO_PIN_4
#define WHEEL_PIN_3_1 GPIO_PIN_5
#define WHEEL_PIN_3_2 GPIO_PIN_6
#define WHEEL_PIN_4_1 GPIO_PIN_7
#define WHEEL_PIN_4_2 GPIO_PIN_8
#define WHEEL_PORT GPIOF
#define WHEEL_PIN_ENABLE  __HAL_RCC_GPIOF_CLK_ENABLE


// 机械臂电机
extern arm_servo as1, as2, as3, as4, as5;


// 巡线的黑白线检测反转
#define TRACKING_REVERSE 0
// 巡线模块的GPIO定义
#define TRACKING_PIN_1 GPIO_PIN_3
#define TRACKING_PIN_2 GPIO_PIN_4
#define TRACKING_PIN_3 GPIO_PIN_5
#define TRACKING_PIN_4 GPIO_PIN_6
#define TRACKING_PORT GPIOG
#define TRACKING_PIN_ENABLE  __HAL_RCC_GPIOG_CLK_ENABLE


#define BASE_LEFT_ANGLE   0     // 基座左转角度
#define BASE_RIGHT_ANGLE  180   // 基座右转角度
#define GRAB_ANGLE       30     // 夹持器闭合角度
#define RELEASE_ANGLE    150    // 夹持器张开角度
#define RETRACT_AS2      20     // 收起时舵机2角度
#define RETRACT_AS3      160    // 收起时舵机3角度
#define RETRACT_AS4      90     // 收起时舵机4角度


#endif // !__SETTING_INCLUDE__
