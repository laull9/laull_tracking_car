#ifndef __SETTING_INCLUDE__
#define __SETTING_INCLUDE__
#include "laullstm32.h"

// 轮子速度
typedef struct{
    uint8_t forward, backward, moveleft, moveright, turnleft, turnright;
}wheel_speed;

typedef enum {
    choice_stop, choice_forward, choice_backward, choice_turnleft, choice_turnright, 
    choice_moveleft, choice_moveright, choice_turnleft2, choice_turnright2,
    choice_other
}line_following_choice;

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
extern arm_servo as1, as2 , as3, as4, as5;


// 巡线的黑白线检测反转
#define TRACKING_REVERSE 1
// 巡线模块的GPIO定义
#define TRACKING_PIN_1 GPIO_PIN_3
#define TRACKING_PIN_2 GPIO_PIN_4
#define TRACKING_PIN_3 GPIO_PIN_5
#define TRACKING_PIN_4 GPIO_PIN_6
#define TRACKING_PORT GPIOG
#define TRACKING_PIN_ENABLE  __HAL_RCC_GPIOG_CLK_ENABLE

// 直角转弯时的后退延时、转向延时
extern u16  turn_delay_bw;
extern u16  turn_delay_turn;

#define GRAB_ANGLE       140     // 夹持器闭合角度
#define RELEASE_ANGLE    90      // 夹持器张开角度
#define RETRACT_AS2      100     // 收起时舵机2角度
#define RELEASE_AS2      30


#endif // !__SETTING_INCLUDE__
