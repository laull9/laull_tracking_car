// 需要编码为GB2312

#include "demo.h"

//-----------------------------------------------------------
//-----------------------GPIO接线表---------------------------
//     电机驱动板 1: 左右前轮
//     轮子1:  正负极连F1 F2       PWM连tim2-3 B10        输出
//     轮子2:  正负极连F3 F4       PWM连tim2-3 B10        输出
//     电机驱动板 2: 左右后轮
//     轮子3:  正负极连F5 F6       PWM连tim2-3 B10        输出
//     轮子4:  正负极连F7 F8       PWM连tim2-3 B10        输出
//     
//     机械臂-舵机1:  正负极连5v电源     PWM连tim2-1  A0   输出
//     机械臂-舵机2:  正负极连5v电源     PWM连tim2-2  A1   输出
//     机械臂-舵机3:  正负极连5v电源     PWM连tim2-4  B11  输出
//     机械臂-舵机4:  正负极连5v电源     PWM连tim3-3  B0   输出
//     机械臂-舵机5:  正负极连5v电源     PWM连tim3-4  B1   输出
//
//     循迹模块--4输入    D11 D12 D13 D14                 输入
//-----------------------------------------------------------
//-----------------------------------------------------------

int main(){
    HAL_Init();
	sys_stm32_clock_init(RCC_PLL_MUL9);
	delay_init(72);
	usart_init(115200);


    laull_tim_init(tim2, 2, 1, 20000, 72-1, TIM_OCPOLARITY_HIGH); // A0
    laull_tim_init(tim2, 2, 2, 20000, 72-1, TIM_OCPOLARITY_HIGH); // A1
    laull_tim_init(tim2, 2, 3, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B10
    laull_tim_init(tim2, 2, 4, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B11
    laull_tim_init(tim3, 3, 3, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B0
    laull_tim_init(tim3, 3, 4, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B1


    TIM_SET_DUTY_CYCLE(tim2, 1, 20);
    TIM_SET_DUTY_CYCLE(tim2, 2, 40);
    TIM_SET_DUTY_CYCLE(tim2, 3, 60);
    TIM_SET_DUTY_CYCLE(tim2, 4, 80);
    TIM_SET_DUTY_CYCLE(tim3, 3, 50);
    TIM_SET_DUTY_CYCLE(tim3, 4, 90);
    
}