#include "demo.h"


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


void demo_gpio_init(){
    // 初始化四个轮子的GPIO
    WHEEL_PIN_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin =
    WHEEL_PIN_1_1 | WHEEL_PIN_1_2 | WHEEL_PIN_2_1 | WHEEL_PIN_2_2 |
    WHEEL_PIN_3_1 | WHEEL_PIN_3_2 | WHEEL_PIN_4_1 | WHEEL_PIN_4_2 ;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // 推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(WHEEL_PORT, &GPIO_InitStruct);

    // 初始化循迹模块的GPIO
    TRACKING_PIN_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct_2 = {0};
    GPIO_InitStruct.Pin =
    TRACKING_PIN_1 | TRACKING_PIN_2 |
    TRACKING_PIN_3 | TRACKING_PIN_4 ;
    GPIO_InitStruct_2.Mode = GPIO_MODE_AF_INPUT; // 推挽输入
    GPIO_InitStruct_2.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TRACKING_PORT, &GPIO_InitStruct_2);
}

void demo_tim_init(){
    laull_tim_init(tim2, 2, 1, 20000, 72-1, TIM_OCPOLARITY_HIGH); // A0
    laull_tim_init(tim2, 2, 2, 20000, 72-1, TIM_OCPOLARITY_HIGH); // A1
    laull_tim_init(tim2, 2, 3, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B10
    laull_tim_init(tim2, 2, 4, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B11
    laull_tim_init(tim3, 3, 3, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B0
    laull_tim_init(tim3, 3, 4, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B1
}
