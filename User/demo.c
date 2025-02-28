#include "demo.h"

// 共享变量定义
TIM_HandleTypeDef tim2, tim3;
wheel w1 = {GPIO_PIN_1, GPIO_PIN_2, GPIOF, GPIOF};
wheel w2 = {GPIO_PIN_3, GPIO_PIN_4, GPIOF, GPIOF};
wheel w3 = {GPIO_PIN_5, GPIO_PIN_6, GPIOF, GPIOF};
wheel w4 = {GPIO_PIN_7, GPIO_PIN_8, GPIOF, GPIOF};


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM2)

    {
        GPIO_InitTypeDef gpio_init_struct;
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_TIM2_CLK_ENABLE();
        gpio_init_struct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);
        gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM2_PARTIAL_2();  // Partial remap (CH1/ETR/PA0,  CH2/PA1, CH3/PB10, CH4/PB11)

    }

    if (htim->Instance == TIM3)

    {
        GPIO_InitTypeDef gpio_init_struct;
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_TIM3_CLK_ENABLE();
        gpio_init_struct.Pin = GPIO_PIN_5 | GPIO_PIN_4 ;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);
        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM3_PARTIAL();  // Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)

    }
}

uint16_t angle_to_tim_val(float angle)
{
    uint16_t ret;
    if((angle < 0)||(angle > 180))
    {
        ret = 0;
    }
    else
    {
        ret = (1500 + (int)((float)((angle - 90) * 100 / 9)));
        if((ret < 500)||(ret > 2500))
        {
            ret = 0;
        }
    }
    return ret;
}


uint8_t read_tracking_sensor(uint16_t pin) {
    uint8_t value = HAL_GPIO_ReadPin(TRACKING_PORT, pin);
    if (TRACKING_REVERSE) {
        value = !value;
    }
    return value;
}


void car_line_following_control() {
    uint8_t farleft = read_tracking_sensor(TRACKING_PIN_1);
    uint8_t midleft = read_tracking_sensor(TRACKING_PIN_2);
    uint8_t midright = read_tracking_sensor(TRACKING_PIN_3);
    uint8_t farright = read_tracking_sensor(TRACKING_PIN_4);

    // 根据传感器值控制车辆运动
    if (farleft && midleft && midright && farright) {
        // 四个传感器都检测到黑线，停止
        car_stop();
    } else if (!farleft && !midleft && !midright && !farright) {
        // 四个传感器都检测到白线，直走
        car_go_forward();
    } else if (farleft && !midleft && !midright && !farright) {
        // 最左边传感器检测到黑线，左转
        car_turn_left();
    } else if (!farleft && !midleft && !midright && farright) {
        // 最右边传感器检测到黑线，右转
        car_turn_right();
    } else if (farleft && midleft && !midright && !farright) {
        // 左边两个传感器检测到黑线，左转
        car_turn_left();
    } else if (!farleft && !midleft && midright && farright) {
        // 右边两个传感器检测到黑线，右转
        car_turn_right();
    } else {
        // 其他情况，直走
        car_go_forward();
    }
}
