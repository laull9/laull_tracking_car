#include "./USMART/usmart.h"
#include "./USMART/usmart_str.h"

/******************************************************************************************/
/* 用户配置区
 * 这下面要包含所用到的函数所申明的头文件(用户自己添加)
 */
 
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/ATK_MO1053/atk_mo1053.h"


/* 函数名列表初始化(用户自己添加)
 * 用户直接在这里输入要执行的函数名及其查找串
 */
struct _m_usmart_nametab usmart_nametab[] =
{
#if USMART_USE_WRFUNS == 1      /* 如果使能了读写操作 */
    (void *)read_addr, "uint32_t read_addr(uint32_t addr)",
    (void *)write_addr, "void write_addr(uint32_t addr,uint32_t val)",
#endif
    (void *)delay_ms, "void delay_ms(uint16_t nms)",
    (void *)delay_us, "void delay_us(uint32_t nus)",

    (void *)atk_mo1053_set_volume, "void vs10xx_set_volume(uint8_t volx)",
    (void *)atk_mo1053_set_tone, "void vs10xx_set_tone(uint8_t bfreq, uint8_t bass, uint8_t tfreq, uint8_t treble)",
    (void *)atk_mo1053_set_effect, "void vs10xx_set_effect(uint8_t effect)",
    (void *)atk_mo1053_get_byterate, "uint16_t vs10xx_get_byterate(void)",
    (void *)atk_mo1053_get_bitrate, "uint16_t vs10xx_get_bitrate(void)",
    (void *)atk_mo1053_set_speaker, "void vs10xx_set_speaker(uint8_t sw)",

};

/******************************************************************************************/

/* 函数控制管理器初始化
 * 得到各个受控函数的名字
 * 得到函数总数量
 */
struct _m_usmart_dev usmart_dev =
{
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), /* 函数数量 */
    0,      /* 参数数量 */
    0,      /* 函数ID */
    1,      /* 参数显示类型,0,10进制;1,16进制 */
    0,      /* 参数类型.bitx:,0,数字;1,字符串 */
    0,      /* 每个参数的长度暂存表,需要MAX_PARM个0初始化 */
    0,      /* 函数的参数,需要PARM_LEN个0初始化 */
};



















