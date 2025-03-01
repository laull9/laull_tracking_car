// 需要编码为GB2312

#include "demo.h"
#include "ov7725.h"

// TIM定义
TIM_HandleTypeDef tim2, tim3;

//-----------------------------------------------------------
//-----------------------GPIO接线表---------------------------
//     电机驱动板 1: 左右前轮
//     轮子1:  正负极连F1 F2       PWM连tim2-3 B10        输出
        wheel w1 = {GPIO_PIN_1, GPIO_PIN_2, GPIOF, GPIOF};
//     轮子2:  正负极连F3 F4       PWM连tim2-3 B10        输出
        wheel w2 = {GPIO_PIN_3, GPIO_PIN_4, GPIOF, GPIOF};
//     电机驱动板 2: 左右后轮
//     轮子3:  正负极连F5 F6       PWM连tim2-3 B10        输出
        wheel w3 = {GPIO_PIN_5, GPIO_PIN_6, GPIOF, GPIOF};
//     轮子4:  正负极连F7 F8       PWM连tim2-3 B10        输出
        wheel w4 = {GPIO_PIN_7, GPIO_PIN_8, GPIOF, GPIOF};
//     
//     机械臂-舵机1:  正负极连5v电源     PWM连tim2-1  A0   输出
        arm_servo as1 = {&tim2, TIM_CHANNEL_1};
//     机械臂-舵机2:  正负极连5v电源     PWM连tim2-2  A1   输出
        arm_servo as2 = {&tim2, TIM_CHANNEL_2};
//     机械臂-舵机3:  正负极连5v电源     PWM连tim2-4  B11  输出
        arm_servo as3 = {&tim2, TIM_CHANNEL_4};
//     机械臂-舵机4:  正负极连5v电源     PWM连tim3-3  B0   输出
        arm_servo as4 = {&tim3, TIM_CHANNEL_3};
//     机械臂-舵机5:  正负极连5v电源     PWM连tim3-4  B1   输出
        arm_servo as5 = {&tim3, TIM_CHANNEL_4};
//
//     循迹模块--4输入    D11 D12 D13 D14                 输入
//-----------------------------------------------------------
//-----------------------------------------------------------

int main(){
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);

    demo_gpio_init();
    demo_tim_init();

    car_set_speed(90);
    
}