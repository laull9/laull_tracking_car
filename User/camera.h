#ifndef __CAMERA_INCLUDE__
#define __CAMERA_INCLUDE__

#include "laullstm32.h"
#include "ov7725.h"

typedef enum {
    camera_unknown,
    camera_white,  camera_black,  camera_red, 
    camera_orange, camera_yellow, camera_green, 
    camera_cyan,   camera_blue,   camera_purple
} camera_Color;

extern uint8_t g_ov7725_vsta;       // 相机帧中断标记 在exit.c里定义

static uint16_t g_ov7725_wwidth  = 320;  // 相机处理宽度
static uint16_t g_ov7725_wheight = 240;  // 相机处理高度



#endif // !__CAMERA_INCLUDE__
