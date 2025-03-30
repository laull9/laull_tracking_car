// ��Ҫ����ΪGB2312

#include "demo.h"

// TIM����
TIM_HandleTypeDef tim2, tim3, tim4;
// UART����ͨѶ: TX-B10 RX-B11

//-----------------------------------------------------------
//-----------------------GPIO���߱�---------------------------
//     ��������� 1: ����ǰ��
//     ����1:  ��������F1 F2       PWM��tim4-1 D12        ���
        wheel w1 = {WHEEL_PIN_1_1, WHEEL_PIN_1_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_1, 
        /*�����ٶ�����*/{/*forward*/40, /*backward*/40, /*moveleft*/40, /*moveright*/40, /*turnleft*/40, /*turnright*/40}};

//     ����2:  ��������F3 F4       PWM��tim4-2 D13        ���
        wheel w2 = {WHEEL_PIN_2_1, WHEEL_PIN_2_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_2, 
        /*�����ٶ�����*/{/*forward*/40, /*backward*/40, /*moveleft*/40, /*moveright*/40, /*turnleft*/40, /*turnright*/40}};
        
//     ��������� 2: ���Һ���
//     ����3:  ��������F5 F6       PWM��tim4-3 D14        ���
        wheel w3 = {WHEEL_PIN_3_1, WHEEL_PIN_3_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_3, 
        /*�����ٶ�����*/{/*forward*/40, /*backward*/40, /*moveleft*/40, /*moveright*/40, /*turnleft*/40, /*turnright*/40}};
        
//     ����4:  ��������F7 F8       PWM��tim4-4 D15        ���
        wheel w4 = {WHEEL_PIN_4_1, WHEEL_PIN_4_2, WHEEL_PORT, WHEEL_PORT, &tim4, TIM_CHANNEL_4, 
        /*�����ٶ�����*/{/*forward*/40, /*backward*/40, /*moveleft*/40, /*moveright*/40, /*turnleft*/40, /*turnright*/40}};
        
  
//     ��е��-���1:  ��������5v��Դ     PWM��tim2-1  A15  ���
        arm_servo as1 = {&tim2, TIM_CHANNEL_1};
//     ��е��-���2:  ��������5v��Դ     PWM��tim2-2  B3   ���
        arm_servo as2 = {&tim2, TIM_CHANNEL_2};
//     ��е��-���3:  ��������5v��Դ     PWM��tim2-4  A3   ���
        arm_servo as3 = {&tim2, TIM_CHANNEL_4};
//     ��е��-���4:  ��������5v��Դ     PWM��tim3-3  B0   ���
        arm_servo as4 = {&tim3, TIM_CHANNEL_3};
//     ��е��-���5:  ��������5v��Դ     PWM��tim3-4  B1   ���
        arm_servo as5 = {&tim3, TIM_CHANNEL_4};
//
//     ѭ��ģ��--4����    G3 G4 G5 G6                     ����
//-----------------------------------------------------------
//-----------------------------------------------------------

void run_mainrace(){
    // ��ʼ�׶Σ������е�ۣ��ſ�ץȡ��

    retract_arm();
    release_object();

    // ����KEY0��ť��ʼץȡ���
    while (key_scan(0) != KEY0){
        delay_ms(20);
    }
    grab_object();

    // ��һ���֣�Ѳ��ֱ����⵽��ɫ���򣬷��û���
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

    // �ڶ����֣�Ѳ��ֱ����⵽��ɫ�����ƶ����ɴ���������
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

    // �������֣�Ѳ��ֱ����⵽��ɫ�����ƶ����ɴ���������
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