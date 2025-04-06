#include "wheel.h"

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
    wheel_run(&w1);
    wheel_run(&w2);
    wheel_run(&w3);
    wheel_run(&w4);
}

void car_go_backward() {
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, backward);
    wheel_runback(&w1);
    wheel_runback(&w2);
    wheel_runback(&w3);
    wheel_runback(&w4);
}

void car_turn_right() {
    // 左前轮和左后轮停止，右前轮和右后轮前进
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, turnleft);
    wheel_stop(&w1);
    wheel_stop(&w3);
    wheel_run(&w2);
    wheel_run(&w4);
}

void car_turn_left() {
    // 右前轮和右后轮停止，左前轮和左后轮前进
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, turnright);
    wheel_stop(&w2);
    wheel_stop(&w4);
    wheel_run(&w1);
    wheel_run(&w3);
}

void car_move_right() {
    // 左前轮后退，右前轮前进，左后轮前进，右后轮后退
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, moveleft);
    wheel_runback(&w1);
    wheel_run(&w2);
    wheel_run(&w3);
    wheel_runback(&w4);
}

void car_move_left() {
    // 左前轮前进，右前轮后退，左后轮后退，右后轮前进
    WHEEL_SET_NOW_SPEED_4(w1, w2, w3, w4, moveright);
    wheel_run(&w1);
    wheel_runback(&w2);
    wheel_runback(&w3);
    wheel_run(&w4);
}

void car_stop() {
    wheel_stop(&w1);
    wheel_stop(&w2);
    wheel_stop(&w3);
    wheel_stop(&w4);
}