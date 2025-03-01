// ��Ҫ����ΪGB2312

#include "demo.h"

// TIM����
TIM_HandleTypeDef tim2, tim3;

//-----------------------------------------------------------
//-----------------------GPIO���߱�---------------------------
//     ��������� 1: ����ǰ��
//     ����1:  ��������F1 F2       PWM��tim2-3 B10        ���
        wheel w1 = {GPIO_PIN_1, GPIO_PIN_2, GPIOF, GPIOF};
//     ����2:  ��������F3 F4       PWM��tim2-3 B10        ���
        wheel w2 = {GPIO_PIN_3, GPIO_PIN_4, GPIOF, GPIOF};
//     ��������� 2: ���Һ���
//     ����3:  ��������F5 F6       PWM��tim2-3 B10        ���
        wheel w3 = {GPIO_PIN_5, GPIO_PIN_6, GPIOF, GPIOF};
//     ����4:  ��������F7 F8       PWM��tim2-3 B10        ���
        wheel w4 = {GPIO_PIN_7, GPIO_PIN_8, GPIOF, GPIOF};
//     
//     ��е��-���1:  ��������5v��Դ     PWM��tim2-1  A0   ���
        arm_servo as1 = {&tim2, TIM_CHANNEL_1};
//     ��е��-���2:  ��������5v��Դ     PWM��tim2-2  A1   ���
        arm_servo as2 = {&tim2, TIM_CHANNEL_2};
//     ��е��-���3:  ��������5v��Դ     PWM��tim2-4  B11  ���
        arm_servo as3 = {&tim2, TIM_CHANNEL_4};
//     ��е��-���4:  ��������5v��Դ     PWM��tim3-3  B0   ���
        arm_servo as4 = {&tim3, TIM_CHANNEL_3};
//     ��е��-���5:  ��������5v��Դ     PWM��tim3-4  B1   ���
        arm_servo as5 = {&tim3, TIM_CHANNEL_4};
//
//     ѭ��ģ��--4����    D11 D12 D13 D14                 ����
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

    // ��ʼ�׶Σ������е�ۣ��ſ�ץȡ��
    car_set_speed(90);
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