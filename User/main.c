// 需要编码为GB2312

#include "demo.h"

#define DEFAULT_SPEED 50

// TIM定义
TIM_HandleTypeDef tim2, tim3, tim4;
// UART串口通讯: TX-B10 RX-B11

//-----------------------------------------------------------
//-----------------------GPIO接线表---------------------------
//     电机驱动板 1: 左右前轮
//     轮子1:  正负极连F1 F2       PWM连tim3-4  B1        输出
        wheel w1 = {WHEEL_PIN_1_1, WHEEL_PIN_1_2, WHEEL_PORT, WHEEL_PORT, &tim3, TIM_CHANNEL_4, 
        /*单轮速度配置*/{/*forward*/DEFAULT_SPEED, /*backward*/DEFAULT_SPEED, /*moveleft*/DEFAULT_SPEED, /*moveright*/DEFAULT_SPEED, /*turnleft*/DEFAULT_SPEED, /*turnright*/DEFAULT_SPEED}};

//     轮子2:  正负极连F3 F4       PWM连tim4-2 D13        输出
        wheel w2 = {WHEEL_PIN_2_1, WHEEL_PIN_2_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_2, 
        /*单轮速度配置*/{/*forward*/DEFAULT_SPEED, /*backward*/DEFAULT_SPEED, /*moveleft*/DEFAULT_SPEED, /*moveright*/DEFAULT_SPEED, /*turnleft*/DEFAULT_SPEED, /*turnright*/DEFAULT_SPEED}};
        
//     电机驱动板 2: 左右后轮
//     轮子3:  正负极连F5 F6       PWM连tim4-3 D14        输出
        wheel w3 = {WHEEL_PIN_3_1, WHEEL_PIN_3_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_3, 
        /*单轮速度配置*/{/*forward*/DEFAULT_SPEED, /*backward*/DEFAULT_SPEED, /*moveleft*/DEFAULT_SPEED, /*moveright*/DEFAULT_SPEED, /*turnleft*/DEFAULT_SPEED, /*turnright*/DEFAULT_SPEED}};
        
//     轮子4:  正负极连F7 F8       PWM连tim4-4 D15        输出
        wheel w4 = {WHEEL_PIN_4_1, WHEEL_PIN_4_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_4, 
        /*单轮速度配置*/{/*forward*/DEFAULT_SPEED, /*backward*/DEFAULT_SPEED, /*moveleft*/DEFAULT_SPEED, /*moveright*/DEFAULT_SPEED, /*turnleft*/DEFAULT_SPEED, /*turnright*/DEFAULT_SPEED}};
        
  
//     机械臂-舵机1:  正负极连5v电源     PWM连tim2-1  A15  输出
        arm_servo as1 = {&tim2, TIM_CHANNEL_1};
//     机械臂-舵机2:  正负极连5v电源     PWM连tim3-3  B0   输出  
        arm_servo as2 = {&tim3, TIM_CHANNEL_3};
//     机械臂-舵机3:  正负极连5v电源     PWM连tim2-4  A3   输出
        arm_servo as3 = {&tim2, TIM_CHANNEL_4};
//     机械臂-舵机4:  正负极连5v电源     PWM连tim2-2  B3   输出
        arm_servo as4 = {&tim2, TIM_CHANNEL_2};
//     机械臂-舵机5:  正负极连5v电源     PWM连tim4-1  D12  输出
        arm_servo as5 = {&tim4, TIM_CHANNEL_1};
//
//     循迹模块--4输入    G3 G4 G5 G6                     输入
//-----------------------------------------------------------
//-----------------------------------------------------------

void run_mainrace(){
    const u8 TIME_STEP = 20;
    const u32 TIME_TURN1 = 4.7 * 1000;
    const u32 TIME_TURN2 = 0.8 * 1000;
    const u32 TIME_OBJECT = 1.65 * 1000;
    const u32 TIME_BIDA = 2.4 * 1000;
    const u32 TIME_FILLAY = 1.7 * 1000;
    const u32 TIME_MOVE_RIGHT = 0.8 * 1000;
    // 开始阶段，收起机械臂，张开抓取夹
    // as2_up();
    arm_servo_set_angle(&as2, RETRACT_AS2);
    release_object();

    // 按下KEY0按钮开始抓取物块
    while (key_scan(0) != WKUP_PRES){
        delay_ms(200);
    }
    grab_object();
    delay_ms(1000);
    // 过第一个直角弯
    line_following_choice c;
    for (u32 i = 0; i < TIME_TURN1; i += TIME_STEP){
        c = car_line_following_control();
        if (c == choice_turnright2){
            car_turn_right_2();
            break;
        }
        delay_ms(TIME_STEP);
    }
    if(c != choice_turnright2){
        car_turn_right_2();
    }

    // 过第二个直角弯
    turn_delay_bw -= 50;
    turn_delay_turn -= 70;
    for (u32 i = 0; i < TIME_TURN2; i += TIME_STEP){
        c = car_line_following_control();
        if (c == choice_turnright2){
            car_turn_right_2();
            break;
        }
        delay_ms(TIME_STEP);
    }
    if(c != choice_turnright2){
        car_turn_right_2();
    }
    for (u32 i = 0; i < TIME_OBJECT; i += TIME_STEP){
        car_line_following_control();
        delay_ms(TIME_STEP);
    }
    // 释放物体
    car_stop();
    as2_down();
    delay_ms(300);
    release_object();
    delay_ms(500);
    as2_up();

    for (u32 i = 0; i < TIME_BIDA; i += TIME_STEP){
        car_line_following_control();
        delay_ms(TIME_STEP);
    }
    car_stop();
    delay_ms(1000);
    car_go_backward();
    delay_ms(TIME_FILLAY);
    car_stop();
    
    delay_ms(1000);
    car_move_right();
    delay_ms(TIME_MOVE_RIGHT);
    car_stop();
}


int main(){
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);
    key_init();
    led_init();

    // arm_servo_set_angle(&as2, RETRACT_AS2);
    // release_object();

    /* 初始化ATK-MS6050 */
    // u8 ret = atk_ms6050_init();
    // if (ret != 0)
    // {
    //     printf("ATK-MS6050 init failed!\r\n");
    //     while (1)
    //     {
    //         LED0_TOGGLE();
    //         delay_ms(200);
    //     }
    // }

    demo_gpio_init();
    demo_tim_init();

    arm_servo_set_angle(&as2, RETRACT_AS2);
    release_object();

    for(uint8_t k;;){
        k = key_scan(0);
        switch (k)
        {
        case KEY0_PRES:
            run_commanda_loop();
            break;
        
        case WKUP_PRES:
            run_mainrace();
            // while (key_scan(0) != KEY0){
            //     delay_ms(30);
            //     // tly_control(car_line_following_control());
            //     car_line_following_control();
            // }
            break;
        }
    }
    
}