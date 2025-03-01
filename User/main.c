// 需要编码为GB2312

#include "demo.h"

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
    key_init();

    demo_gpio_init();
    demo_tim_init();

    // 开始阶段，收起机械臂，张开抓取夹
    car_set_speed(90);
    retract_arm();
    release_object();

    // 按下KEY0按钮开始抓取物块
    while (key_scan(0) != KEY0){
        delay_ms(20);
    }
    grab_object();

    // 第一部分：巡线直到检测到紫色区域，放置货物
    camera_Color color = camera_unknown;
    while (color != camera_purple){
        car_line_following_control();
        color = camera_scan_color();
    }
    delay_ms(1000 * 1);
    car_stop();
    rotate_base_right();
    release_object();
    delay_ms(1000 * 2);

    // 第二部分：巡线直到检测到红色区域，移动到可达区并返回
    retract_arm();
    car_go_forward();
    while (color != camera_red){
        car_line_following_control();
        color = camera_scan_color();
    }
    delay_ms(1000 * 1);
    uint32_t _timer = 0;
    car_move_right();
    while (color != camera_yellow){
        color = camera_scan_color();
        delay_ms(10);
        _timer++;
    }
    car_move_left();
    while (_timer--){
        color = camera_scan_color();
        delay_ms(10);
    }
    car_stop();

    // 第三部分：巡线直到检测到红色区域，移动到可达区并返回
    while (car_line_following_control() != choice_stop){
        delay_ms(10);
    }
    car_go_forward();
    delay_ms(1000 * 2);
    car_stop();
}