#include "usart_commad.h"

#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define str_equal(s1, s2) (strcmp(s1, s2) == 0)
bool commands_looping = 0;

void _car_time_stop(float time){
    if (time != 0){
        delay_ms(time * 1000);
        car_stop();
    }
}

bool _handle_set_speed(const char* target, float data) {
    size_t len = strlen(target);
    if (len < 3) return 0;
    // 检查是否以'w'开头
    if (target[0] != 'w') return 0;
    // 提取轮子编号（1-4）
    char wheel_num_char = target[1];
    if (wheel_num_char < '1' || wheel_num_char > '4') return 0;
    int wheel_num = wheel_num_char - '0';
    // 获取对应的wheel结构体指针
    wheel* p_wheel;
    switch (wheel_num) {
        case 1: p_wheel = &w1; break;
        case 2: p_wheel = &w2; break;
        case 3: p_wheel = &w3; break;
        case 4: p_wheel = &w4; break;
        default: return 0;
    }
    // 提取动作部分（例如"f"、"mr"）
    const char* action = target + 2;
    
    // 根据动作类型设置对应速度值
    if (str_equal(action, "f")) {
        p_wheel->speed.forward = (uint8_t)data;
    } else if (str_equal(action, "b")) {
        p_wheel->speed.backward = (uint8_t)data;
    } else if (str_equal(action, "ml")) {
        p_wheel->speed.moveleft = (uint8_t)data;
    } else if (str_equal(action, "mr")) {
        p_wheel->speed.moveright = (uint8_t)data;
    } else if (str_equal(action, "tl")) {
        p_wheel->speed.turnleft = (uint8_t)data;
    } else if (str_equal(action, "tr")) {
        p_wheel->speed.turnright = (uint8_t)data;
    }else{
        return 0;
    }
    return 1;
}

void handle_exit(){
    commands_looping = 0;
    printf("exited\n");
    LED1_OFF();
}

    // 处理set命令
void handle_set(const char* target, float data) {
    if(_handle_set_speed(target, data))
        printf("done set\n");;
}

    // 处理run命令
void handle_run(const char* target, float time) {
    if (str_equal(target, "f")){
        car_go_forward();
        _car_time_stop(time);
    }
    else if (str_equal(target, "b")){
        car_go_backward();
        _car_time_stop(time);
    }
    else if (str_equal(target, "ml")){
        car_move_left();
        _car_time_stop(time);
    }
    else if (str_equal(target, "mr")){
        car_move_right();
        _car_time_stop(time);
    }
    else if (str_equal(target, "tl")){
        car_turn_left();
        _car_time_stop(time);
    }
    else if (str_equal(target, "tr")){
        car_turn_right();
        _car_time_stop(time);
    }
    else{
        return;
    }
    printf("done run\n");
}

static float parse_float(const uint8_t *str, uint16_t len) {
    float sign = 1.0f;
    float value = 0.0f;
    float fraction = 0.1f;
    uint16_t i = 0;
    uint8_t in_fraction = 0;

    if (len == 0) return 0.0f;

    // 处理符号
    if (str[i] == '-') {
        sign = -1.0f;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // 解析整数和小数部分
    for (; i < len; i++) {
        if (str[i] == '.') {
            in_fraction = 1;
            continue;
        }

        if (IS_DIGIT(str[i])) {
            if (!in_fraction) {
                value = value * 10.0f + (str[i] - '0');
            } else {
                value += (str[i] - '0') * fraction;
                fraction *= 0.1f;
            }
        }
    }

    return sign * value;
}

void parse_commands(uint8_t* buffer, uint16_t len) {
    uint16_t i = 0;
    while (i < len) {
        // 检查命令类型
        if (i + 3 < len && 
            buffer[i]   == 's' &&
            buffer[i+1] == 'e' &&
            buffer[i+2] == 't' &&
            buffer[i+3] == ' ') 
        {
            i += 4;  // 跳过"set "
            
            // 提取目标字符串
            char target[32] = {0};
            uint8_t t_idx = 0;
            while (i < len && buffer[i] != ' ' && t_idx < sizeof(target)-1) {
                target[t_idx++] = buffer[i++];
            }
            target[t_idx] = '\0';
            
            if (i >= len) break;
            i++;  // 跳过空格
            
            // 提取数值字符串
            char num_str[32] = {0};
            uint8_t n_idx = 0;
            while (i < len && 
                  (IS_DIGIT(buffer[i]) || 
                   buffer[i] == '.' || 
                   buffer[i] == '-' ||
                   buffer[i] == '+') && 
                  n_idx < sizeof(num_str)-1) 
            {
                num_str[n_idx++] = buffer[i++];
            }
            num_str[n_idx] = '\0';
            
            // 转换并处理命令
            handle_set(target, parse_float((uint8_t*)num_str, n_idx));
        }
        else if (i + 3 < len && 
                buffer[i]   == 'r' &&
                buffer[i+1] == 'u' &&
                buffer[i+2] == 'n' &&
                buffer[i+3] == ' ') 
        {
            i += 4;  // 跳过"run "
            
            // 提取目标字符串
            char target[32] = {0};
            uint8_t t_idx = 0;
            while (i < len && buffer[i] != ' ' && t_idx < sizeof(target)-1) {
                target[t_idx++] = buffer[i++];
            }
            target[t_idx] = '\0';
            
            if (i >= len) break;
            i++;  // 跳过空格
            
            // 提取时间字符串
            char time_str[32] = {0};
            uint8_t t_idx2 = 0;
            while (i < len && 
                  (IS_DIGIT(buffer[i]) || 
                   buffer[i] == '.' || 
                   buffer[i] == '-' ||
                   buffer[i] == '+') && 
                  t_idx2 < sizeof(time_str)-1) 
            {
                time_str[t_idx2++] = buffer[i++];
            }
            time_str[t_idx2] = '\0';
            
            // 转换并处理命令
            handle_run(target, parse_float((uint8_t*)time_str, t_idx2));
        }
        else if (i + 3 < len && 
            buffer[i]   == 'e' &&
            buffer[i+1] == 'x' &&
            buffer[i+2] == 'i' &&
            buffer[i+3] == 't')
        {
            i += 4;
            handle_exit();
        }
        else {
            i++;  // 继续查找
        }
    }
}

void run_commanda_loop(){
    commands_looping = 1;
    uint8_t c_led = 0;
    while (commands_looping)
    {
        if (g_usart_rx_sta & 0x8000)        /* 接收到了数据? */
        {
            uint16_t len = g_usart_rx_sta & 0x3fff;
            parse_commands(g_usart_rx_buf, len);
            g_usart_rx_sta = 0;
            for (uint16_t i = 0; i < len ; ++i){
                g_usart_rx_buf[i] = 0;
            }
        }
        delay_ms(10);
        if(++c_led == 50){
            LED1_TOGGLE();
            c_led = 0;
        }
    }
    
}