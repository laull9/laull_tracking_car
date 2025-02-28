#include"laullstm32.h"

void chinese_init(){
    exfuns_init();                      /* 为fatfs相关变量申请内存 */
    fonts_init();
}

bool scan_touched(u16 *x, u16 *y){
    tp_dev.scan(0);
    if (tp_dev.sta & TP_PRES_DOWN)  /* 触摸屏被按下 */
    {
        *x = tp_dev.x[0];
        *y = tp_dev.y[0];
        return true;
    }
    else{
        return false;
    }
}

void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (x1 < size || x2 < size || y1 < size || y2 < size)
        return;

    delta_x = x2 - x1; /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1; /* 设置单步方向 */
    }
    else if (delta_x == 0)
    {
        incx = 0; /* 垂直线 */
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0; /* 水平线 */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x; /* 选取基本增量坐标轴 */
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++) /* 画线输出 */
    {
        lcd_fill_circle(row, col, size, color); /* 画点 */
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

void do_touch_adjust()
{
    lcd_clear(WHITE);   /* 清屏 */
    tp_adjust();        /* 屏幕校准 */
    tp_save_adjust_data();
}

void filesystem_init()
{
    exfuns_init(); 
    // 设置盘符    
    f_mount(fs[0], "0:", 1);
    f_mount(fs[1], "1:", 1);
}

#ifdef __cplusplus

bool get_dir_all_files(const char * dirpath, std::vector<char *> &out_paths, u8 mode){
    out_paths.clear();
    FILINFO *fileinfo = (FILINFO *)mymalloc(SRAMIN, sizeof(FILINFO));
    if(! fileinfo)
        return false; // 内存申请失败
    DIR dir;
    uint8_t res = f_opendir(&dir, dirpath);
    size_t dirpath_len = strlen(dirpath) * sizeof(char);
    if (res == FR_OK)
    {
        int count = 0;
        while (1)
        {
            res = f_readdir(&dir, fileinfo);
            if (res != FR_OK || fileinfo->fname[0] == 0)
                break;
            char * ss;
            if (mode == 0){
            ss = (char *)mymalloc(SRAMIN, strlen(fileinfo->fname) * sizeof(char));
            strcpy( ss , fileinfo->fname);
            
            }
            else{
                ss = (char *)mymalloc(SRAMIN, strlen(fileinfo->fname) * sizeof(char) + dirpath_len + 2);
                if(dirpath[dirpath_len - 1] == '/' )
                    sprintf(ss, "%s%s", dirpath, fileinfo->fname);
                else
                    sprintf(ss, "%s/%s", dirpath, fileinfo->fname);
            }
            out_paths.push_back(ss);
        }
    }
    else{
        myfree(SRAMIN, fileinfo);
        return false;
    }
    myfree(SRAMIN, fileinfo);
    return true;
}

void free_filepaths(std::vector<char *> &out_paths){
    for (auto &i : out_paths){
        myfree(SRAMIN, i);
    }
}


bool scan_touched(u16 &x, u16 &y){
    tp_dev.scan(0);
    if (tp_dev.sta & TP_PRES_DOWN)  /* 触摸屏被按下 */
    {
        x = tp_dev.x[0];
        y = tp_dev.y[0];
        return true;
    }
    else{
        return false;
    }
}


#endif