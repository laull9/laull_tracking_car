#include "demo.h"

uint16_t turn_delay_bw = 500, turn_delay_turn = 1500;

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM2)

    {
        GPIO_InitTypeDef gpio_init_struct = {0};

        // 使能相关时钟
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_AFIO_CLK_ENABLE(); // 开启AFIO时钟

        // 禁用JTAG以释放PA15、PB3、PB4
        __HAL_AFIO_REMAP_SWJ_NOJTAG();

        // 应用TIM2的部分重映射1
        __HAL_AFIO_REMAP_TIM2_PARTIAL_1();

        // 配置GPIO参数
        gpio_init_struct.Mode = GPIO_MODE_AF_PP; // 复用推挽输出
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;

        // 初始化PB3 (TIM2_CH2)
        gpio_init_struct.Pin = GPIO_PIN_3;
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);

        // 初始化PA15 (TIM2_CH1)、PA2 (TIM2_CH3)、PA3 (TIM2_CH4)
        gpio_init_struct.Pin = GPIO_PIN_15 | GPIO_PIN_2 | GPIO_PIN_3;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    }

    if (htim->Instance == TIM3)

    {
        GPIO_InitTypeDef gpio_init_struct;
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_TIM3_CLK_ENABLE();
        gpio_init_struct.Pin = GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_0 | GPIO_PIN_1;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);
        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM3_PARTIAL();  // Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)

    }

    if (htim->Instance == TIM4)

    {
        GPIO_InitTypeDef gpio_init_struct;
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_TIM4_CLK_ENABLE();
        gpio_init_struct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOD, &gpio_init_struct);
        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM4_ENABLE();  // Full remap (CH1/PD12, CH2/PD13, CH3/PD14, CH4/PD15)

    }
}


uint8_t read_tracking_sensor(uint16_t pin) {
    uint8_t value = HAL_GPIO_ReadPin(TRACKING_PORT, pin);
    if (TRACKING_REVERSE) {
        value = !value;
    }
    return value;
}


line_following_choice car_line_following_control() {
    uint8_t farleft = read_tracking_sensor(TRACKING_PIN_1);
    uint8_t midleft = read_tracking_sensor(TRACKING_PIN_2);
    uint8_t midright = read_tracking_sensor(TRACKING_PIN_3);
    uint8_t farright = read_tracking_sensor(TRACKING_PIN_4);

    // 根据传感器值控制车辆运动
    if (farleft && midleft && midright && farright) {
        // 四个传感器都检测到白线，直走
        car_go_forward();
        return choice_forward;
    } else if (!farleft && !midleft && !midright && !farright) {
        // 四个传感器都检测到黑线，停止
        car_stop();
        return choice_stop;
    } else if (farleft && !midleft && !midright && !farright) {
        // 最左边传感器检测到白线，右转
        // 直角弯
        car_go_backward();
        delay_ms(turn_delay_bw);
        car_turn_right();
        delay_ms(turn_delay_turn);
        return choice_turnright;

    } else if (!farleft && !midleft && !midright && farright) {
        // 最右边传感器检测到白线，左转
        // 直角弯
        car_go_backward();
        delay_ms(turn_delay_bw);
        car_turn_left();
        delay_ms(turn_delay_turn);
        return choice_turnleft;

    } else if (!farleft && midleft && midright && farright) {
        // 左边一个传感器检测到黑线，左转
        car_turn_left();
        return choice_turnleft;
    } else if (farleft && midleft && midright && !farright) {
        // 右边一个传感器检测到黑线，右转
        car_turn_right();
        return choice_turnright;
    }
    else {
        // 其他情况，直走
        car_go_forward();
        return choice_other;
    }
}


static line_following_choice last_choice = choice_stop;
static int16_t target_gz = 0;
static float integral = 0;
static int16_t prev_error = 0;

void tly_control(line_following_choice choice) {
    int16_t gx, gy, gz;
    float adjust = 0, base_speed = 0;

    // 获取陀螺仪数据（Z轴为偏航轴）
    atk_ms6050_get_gyroscope(&gx, &gy, &gz);

    // 检测方向变化时重置控制参数
    if (choice != last_choice) {
        target_gz = gz;
        integral = 0;
        prev_error = 0;
        last_choice = choice;
    }

    // 公共PID参数
    const float Kp = 0.8f;
    const float Ki = 0.002f;
    const float Kd = 0.3f;

    // 需要陀螺仪修正的运动模式
    if (choice == choice_forward || choice == choice_backward || 
        choice == choice_moveleft || choice == choice_moveright) {
        // 计算角度偏差
        int16_t error = target_gz - gz;
        
        // PID计算
        float proportional = Kp * error;
        integral += Ki * error;
        float derivative = Kd * (error - prev_error);
        prev_error = error;
        adjust = proportional + integral + derivative;

        // 限幅处理
        adjust = adjust > 30 ? 30 : (adjust < -30 ? -30 : adjust);
    }

    switch (choice) {
        case choice_forward: {
            // 前进时左右轮差速修正           
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w1.speed.forward - adjust)
                                        , w1, forward);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w2.speed.forward + adjust)
                                        , w2, forward);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w3.speed.forward + adjust)
                                        , w3, forward);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w4.speed.forward - adjust)
                                        , w4, forward);
            break;
        }

        case choice_backward: {
            // 后退时左右轮反向差速修正
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w1.speed.backward + adjust)
                                        , w1, backward);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w2.speed.backward - adjust)
                                        , w2, backward);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w3.speed.backward - adjust)
                                        , w3, backward);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w4.speed.backward + adjust)
                                        , w4, backward);
            break;
        }

        case choice_moveleft: {
            // 左平移时前后轮差速修正
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w1.speed.moveleft + adjust)
                                        , w1, moveleft);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w2.speed.moveleft - adjust)
                                        , w2, moveleft);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w3.speed.moveleft - adjust)
                                        , w3, moveleft);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w4.speed.moveleft + adjust)
                                        , w4, moveleft);
            break;
        }

        case choice_moveright: {
            // 右平移时前后轮反向差速修正
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w1.speed.moveright - adjust)
                                        , w1, moveright);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w2.speed.moveright + adjust)
                                        , w2, moveright);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w3.speed.moveright + adjust)
                                        , w3, moveright);
            WHEEL_SET_SPEED_IMMIDIATELY(CLAMP_SPEED(w4.speed.moveright - adjust)
                                        , w4, moveright);
            break;
        }

        default:
            break;
    }
}


void demo_gpio_init(){
    // 初始化四个轮子的GPIO
    WHEEL_PIN_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin =
    WHEEL_PIN_1_1 | WHEEL_PIN_1_2 | WHEEL_PIN_2_1 | WHEEL_PIN_2_2 |
    WHEEL_PIN_3_1 | WHEEL_PIN_3_2 | WHEEL_PIN_4_1 | WHEEL_PIN_4_2 ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
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
    laull_tim_init(tim2, 2, 1, 20000, 72-1, TIM_OCPOLARITY_HIGH); // A15
    laull_tim_init(tim2, 2, 2, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B3
    laull_tim_init(tim2, 2, 3, 20000, 72-1, TIM_OCPOLARITY_HIGH); // A2
    laull_tim_init(tim2, 2, 4, 20000, 72-1, TIM_OCPOLARITY_HIGH); // A3
    laull_tim_init(tim3, 3, 1, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B4
    laull_tim_init(tim3, 3, 2, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B5
    laull_tim_init(tim3, 3, 3, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B0
    laull_tim_init(tim3, 3, 4, 20000, 72-1, TIM_OCPOLARITY_HIGH); // B1
    laull_tim_init(tim4, 4, 1, 20000, 72-1, TIM_OCPOLARITY_HIGH); // D12
    laull_tim_init(tim4, 4, 2, 20000, 72-1, TIM_OCPOLARITY_HIGH); // D13
    laull_tim_init(tim4, 4, 3, 20000, 72-1, TIM_OCPOLARITY_HIGH); // D14
    laull_tim_init(tim4, 4, 4, 20000, 72-1, TIM_OCPOLARITY_HIGH); // D15
}
