#include "wheel.h"

u16  turn_delay_bw = 280;
u16  turn_delay_turn = 850;

void wheel_init(wheel* w){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = w->pin_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(w->pin_mode_0, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = w->pin_1;
    HAL_GPIO_Init(w->pin_mode_1, &GPIO_InitStruct);
}

void wheel_stop(wheel* w){
    HAL_GPIO_WritePin(w->pin_mode_0, w->pin_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(w->pin_mode_1, w->pin_1, GPIO_PIN_RESET);
}

void wheel_run(wheel* w){
    HAL_GPIO_WritePin(w->pin_mode_0, w->pin_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(w->pin_mode_1, w->pin_1, GPIO_PIN_RESET);
}

void wheel_runback(wheel* w){
    HAL_GPIO_WritePin(w->pin_mode_0, w->pin_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(w->pin_mode_1, w->pin_1, GPIO_PIN_SET);
}


void car_go_forward() {
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, forward);
    wheel_run(&w2);
    wheel_run(&w1);
    wheel_run(&w3);
    wheel_run(&w4);
}

void car_go_backward() {
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, backward);
    wheel_runback(&w2);
    wheel_runback(&w1);
    wheel_runback(&w3);
    wheel_runback(&w4);
}

void car_turn_left() {
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, turnleft);
    wheel_run(&w2);
    wheel_stop(&w1);
    wheel_stop(&w3);
    wheel_run(&w4);
}

void car_turn_right() {
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, turnright);
    wheel_stop(&w2);
    wheel_stop(&w4);
    wheel_run(&w1);
    wheel_run(&w3);
}

void car_move_left() {
    // 左前轮后退，右前轮前进，左后轮前进，右后轮后退
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, moveleft);
    wheel_run(&w2);
    wheel_runback(&w1);
    wheel_run(&w3);
    wheel_runback(&w4);
}

void car_move_right() {
    // 左前轮前进，右前轮后退，左后轮后退，右后轮前进
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, moveright);
    wheel_runback(&w2);
    wheel_run(&w1);
    wheel_runback(&w3);
    wheel_run(&w4);
}

void car_stop() {
    wheel_stop(&w1);
    wheel_stop(&w2);
    wheel_stop(&w3);
    wheel_stop(&w4);
}

void car_turn_right_2(){
    car_go_backward();
    delay_ms(turn_delay_bw);
    car_turn_right();
    delay_ms(turn_delay_turn);
    car_stop();
}

void car_turn_left_2(){
    car_go_backward();
    delay_ms(turn_delay_bw);
    car_turn_left();
    delay_ms(turn_delay_turn);
    car_stop();
}



static line_following_choice last_choice = choice_stop;
static int16_t target_gz = 0;
static float integral = 0;
static int16_t prev_error = 0;
static int16_t filtered_gz = 0;  // 滤波状态

void tly_control(line_following_choice choice) {
    int16_t gx, gy, gz;
    float adjust = 0;

    // 获取陀螺仪数据并滤波
    atk_ms6050_get_gyroscope(&gx, &gy, &gz);
    const float alpha = 0.3f;
    filtered_gz = (int16_t)(alpha * gz + (1-alpha) * filtered_gz);

    // 方向变化时重置控制参数
    if (choice != last_choice) {
        target_gz = filtered_gz;  // 使用滤波后的值
        integral = 0;
        prev_error = 0;
        last_choice = choice;
    }

    // 调整后的PID参数
    const float Kp = 0.4f;
    const float Ki = 0.0005f;
    const float Kd = 0.1f;
    const float integral_max = 50.0f;
    const int16_t error_threshold = 20;

    if (choice == choice_forward || choice == choice_backward || 
        choice == choice_moveleft || choice == choice_moveright) {
        
        int16_t error = target_gz - filtered_gz;
        
        // 积分分离
        if (abs(error) <= error_threshold) {
            integral += Ki * error;
            integral = (integral > integral_max) ? integral_max : 
                      ((integral < -integral_max) ? -integral_max : integral);
        } else {
            integral = 0;
        }

        float proportional = Kp * error;
        float derivative = Kd * (error - prev_error);
        prev_error = error;
        adjust = proportional + integral + derivative;

        // 缩小输出限幅
        adjust = adjust > 15 ? 15 : (adjust < -15 ? -15 : adjust);
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
