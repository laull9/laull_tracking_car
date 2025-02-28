// ��Ҫ����ΪGB2312

#include "demo.h"

//-----------------------------------------------------------
//-----------------------GPIO���߱�---------------------------
//     ��������� 1: ����ǰ��
//     ����1:  ��������F1 F2       PWM��tim2-3 B10        ���
//     ����2:  ��������F3 F4       PWM��tim2-3 B10        ���
//     ��������� 2: ���Һ���
//     ����3:  ��������F5 F6       PWM��tim2-3 B10        ���
//     ����4:  ��������F7 F8       PWM��tim2-3 B10        ���
//     
//     ��е��-���1:  ��������5v��Դ     PWM��tim2-1  A0   ���
//     ��е��-���2:  ��������5v��Դ     PWM��tim2-2  A1   ���
//     ��е��-���3:  ��������5v��Դ     PWM��tim2-4  B11  ���
//     ��е��-���4:  ��������5v��Դ     PWM��tim3-3  B0   ���
//     ��е��-���5:  ��������5v��Դ     PWM��tim3-4  B1   ���
//
//     ѭ��ģ��--4����    D11 D12 D13 D14                 ����
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