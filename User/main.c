// 需要编码为GB2312

#include "demo.h"

// TIM定义
TIM_HandleTypeDef tim2, tim3, tim4;
// UART串口通讯: TX-B10 RX-B11

//-----------------------------------------------------------
//-----------------------GPIO接线表---------------------------
//     电机驱动板 1: 左右前轮
//     轮子1:  正负极连F1 F2       PWM连tim4-1 D12        输出
        wheel w1 = {WHEEL_PIN_1_1, WHEEL_PIN_1_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_1, 
        /*单轮速度配置*/{/*forward*/40, /*backward*/40, /*moveleft*/40, /*moveright*/40, /*turnleft*/40, /*turnright*/40}};

//     轮子2:  正负极连F3 F4       PWM连tim4-2 D13        输出
        wheel w2 = {WHEEL_PIN_2_1, WHEEL_PIN_2_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_2, 
        /*单轮速度配置*/{/*forward*/40, /*backward*/40, /*moveleft*/40, /*moveright*/40, /*turnleft*/40, /*turnright*/40}};
        
//     电机驱动板 2: 左右后轮
//     轮子3:  正负极连F5 F6       PWM连tim4-3 D14        输出
        wheel w3 = {WHEEL_PIN_3_1, WHEEL_PIN_3_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_3, 
        /*单轮速度配置*/{/*forward*/40, /*backward*/40, /*moveleft*/40, /*moveright*/40, /*turnleft*/40, /*turnright*/40}};
        
//     轮子4:  正负极连F7 F8       PWM连tim4-4 D15        输出
        wheel w4 = {WHEEL_PIN_4_1, WHEEL_PIN_4_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_4, 
        /*单轮速度配置*/{/*forward*/40, /*backward*/40, /*moveleft*/40, /*moveright*/40, /*turnleft*/40, /*turnright*/40}};
        
  
//     机械臂-舵机1:  正负极连5v电源     PWM连tim2-1  A15  输出
        arm_servo as1 = {&tim2, TIM_CHANNEL_1};
//     机械臂-舵机2:  正负极连5v电源     PWM连tim2-2  B3   输出
        arm_servo as2 = {&tim2, TIM_CHANNEL_2};
//     机械臂-舵机3:  正负极连5v电源     PWM连tim2-4  A3   输出
        arm_servo as3 = {&tim2, TIM_CHANNEL_4};
//     机械臂-舵机4:  正负极连5v电源     PWM连tim3-3  B0   输出
        arm_servo as4 = {&tim3, TIM_CHANNEL_3};
//     机械臂-舵机5:  正负极连5v电源     PWM连tim3-4  B1   输出
        arm_servo as5 = {&tim3, TIM_CHANNEL_4};
//
//     循迹模块--4输入    G3 G4 G5 G6                     输入
//-----------------------------------------------------------
//-----------------------------------------------------------

void run_mainrace(){
    // 开始阶段，收起机械臂，张开抓取夹

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


int main(){
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);
    key_init();
    led_init();

    demo_gpio_init();
    demo_tim_init();

    for(uint8_t k;;){
        k = key_scan(0);
        switch (k)
        {
        case KEY0_PRES:
            run_commanda_loop();
            break;
        
        case KEY1_PRES:
            run_mainrace();
            break;
        }
    }
    
}