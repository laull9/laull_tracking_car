#include "camera.h"

camera_Color camera_scan_color()
{
    uint32_t i, j;
    uint16_t color, __color = 0;
 
    if (!g_ov7725_vsta)   // 有帧中断更新
        return camera_unknown;

    OV7725_RRST(0);  // 开始复位读指针
    OV7725_RCLK(0);
    OV7725_RCLK(1);
    OV7725_RCLK(0);
    OV7725_RRST(1);  // 复位读指针结束
    OV7725_RCLK(1);

    // 设计为只读取相机中间9个均匀的采样点来计算最接近的颜色
    int h = g_ov7725_wheight;
    int w = g_ov7725_wwidth;
    int rows[] = {h / 4, h / 2, 3 * h / 4};
    int cols[] = {w / 4, w / 2, 3 * w / 4};
    uint16_t sampled_colors[9] = {0};
    struct Coord { int i, j; } targets[9];
    int idx = 0;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            targets[idx].i = rows[r];
            targets[idx].j = cols[c];
            idx++;
        }
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            OV7725_RCLK(0);
            uint16_t color = OV7725_DATA;
            OV7725_RCLK(1);
            color <<= 8;
            OV7725_RCLK(0);
            color |= OV7725_DATA;
            OV7725_RCLK(1);

            for (int k = 0; k < 9; k++) {
                if (i == targets[k].i && j == targets[k].j) {
                    sampled_colors[k] = color;
                    break;
                }
            }
        }
    }

    // 计算平均颜色
    uint32_t sum_r = 0, sum_g = 0, sum_b = 0;
    for (int k = 0; k < 9; k++) {
        uint16_t c = sampled_colors[k];
        sum_r += (c >> 11) & 0x1F; // R分量（5位）
        sum_g += (c >> 5)  & 0x3F; // G分量（6位）
        sum_b +=  c        & 0x1F; // B分量（5位）
    }
    uint8_t avg_r = sum_r / 9;
    uint8_t avg_g = sum_g / 9;
    uint8_t avg_b = sum_b / 9;

    // 定义纯色列表（RGB565格式）
    typedef struct {
        uint16_t color;
        camera_Color name;
    } PureColor;
    PureColor colors[] = {
        {0xF800, camera_red},    {0xFD20, camera_orange}, {0xFFE0, camera_yellow},
        {0x07E0, camera_green},  {0x07FF, camera_cyan},   {0x001F, camera_blue},
        {0xF81F, camera_purple}, {0x0000, camera_black},  {0xFFFF, camera_white}
    };

    // 找出最接近的颜色
    int min_dist = 0xFFFF;
    camera_Color closest_color = camera_unknown;
    for (int m = 0; m < 9; m++) {
        uint16_t c = colors[m].color;
        int dr = avg_r - ((c >> 11) & 0x1F);
        int dg = avg_g - ((c >> 5)  & 0x3F);
        int db = avg_b - ( c        & 0x1F);
        int dist = dr*dr + dg*dg + db*db;
        if (dist < min_dist) {
            min_dist = dist;
            closest_color = colors[m].name;
        }
    }
    // printf("c   %d\n", color);
    // 清零帧中断标记
    g_ov7725_vsta = 0;
    // 恢复默认扫描方向
    lcd_scan_dir(DFT_SCAN_DIR);
    return closest_color;     // 使用最接近的颜色
}