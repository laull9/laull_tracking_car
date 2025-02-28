/**
 ****************************************************************************************************
 * @file        test.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-17
 * @brief       精英无屏幕测试流程
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200417
 * 第一次发布
 *
 ****************************************************************************************************
 */
#include "./TEST/test.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/IIC/myiic.h"
#include "./MALLOC/malloc.h"
#include "./BSP/LSENS/lsens.h"
#include "./BSP/24CXX/24cxx.h"
#include "./BSP/REMOTE/remote.h"
#include "./MALLOC/malloc.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./BSP/ADC/adc.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h"
/*************************************************************************************************/

USBD_HandleTypeDef USBD_Device;             /* USB Device处理结构体 */
extern volatile uint8_t g_usb_state_reg;    /* USB状态 */
extern volatile uint8_t g_device_state;     /* USB连接 情况 */


/**
 * @brief       USB连接测试
 * @param       无
 * @retval      无
 */
void usb_test(void)
{
    printf("/******************************* 1，USB测试 *******************************/\r\n");
    printf("/* 注意：请将USB线连接到USB_SLAVE接口上，然后观察电脑磁盘是否有出现新的磁盘！ */\r\n");
    printf("/******************************* NEXT: KEY1 *******************************/\r\n\r\n");
    
    uint8_t offline_cnt = 0;
    uint8_t tct = 0;
    uint8_t usb_sta;
    uint8_t device_sta;
    uint16_t id;


    if (sd_init())  /* 初始化SD卡 */
    {
        printf("SD卡初始化失败或者未插卡！！！\r\n");
    }
    else     /* SD 卡正常 */
    {
        printf("SD卡正常，容量大小为：%lldM\r\n",SD_TOTAL_SIZE_MB(&g_sdcard_handler));
    }

    id = norflash_read_id();
    if ((id == 0) || (id == 0XFFFF))
    {
        printf("SPI_FLASH ERROR\r\n");
    }
    else   /* SPI FLASH 正常 */
    {
        printf("SPI_FLASH 正常\r\n");
    }

    usbd_port_config(0);    /* USB先断开 */
    delay_ms(500);
    usbd_port_config(1);    /* USB再次连接 */
    delay_ms(500);

    USBD_Init(&USBD_Device, &MSC_Desc, 0);                              /* 初始化USB */
    USBD_RegisterClass(&USBD_Device, USBD_MSC_CLASS);                   /* 添加类 */
    USBD_MSC_RegisterStorage(&USBD_Device, &USBD_DISK_fops);            /* 为MSC类添加回调函数 */
    USBD_Start(&USBD_Device);                                           /* 开启USB */
    delay_ms(500);

    while (1)
    {
        delay_ms(1);

        if (usb_sta != g_usb_state_reg)   /* 状态改变了 */
        {
            if (g_usb_state_reg & 0x01)   /* 正在写 */
            {
                LED1(0);
                printf("USB Writing...\r\n");
            }

            if (g_usb_state_reg & 0x02)   /* 正在读 */
            {
                LED1(0);
                printf("USB Reading...\r\n");
            }

            if (g_usb_state_reg & 0x04)
            {
                printf("USB Write Err\r\n");
            }
            
            if (g_usb_state_reg & 0x08)
            {
                printf("USB Read  Err \r\n");
            }
            
            usb_sta = g_usb_state_reg; /* 记录最后的状态 */
        }

        if (device_sta != g_device_state)
        {
            if (g_device_state == 1)
            {
                printf("USB连接成功！！\r\n");
            }
            else
            {
                printf("USB连接失败！！\r\n");
            }
            
            device_sta = g_device_state;
        }

        tct++;
        if (key_scan(0) == KEY1_PRES) break;    /* 当按下按键KEY1就退出 */
        if (tct == 200)
        {
            tct = 0;
            LED1(1);        /* 关闭 LED1 */
            LED0_TOGGLE();  /* LED0 闪烁 */

            if (g_usb_state_reg & 0x10)
            {
                offline_cnt = 0;                /* USB连接了,则清除offline计数器 */
                g_device_state = 1;
            }
            else    /* 没有得到轮询 */
            {
                offline_cnt++;

                if (offline_cnt > 100)
                {
                    g_device_state = 0;         /* 20s内没收到在线标记,代表USB被拔出了 */
                }
            }

            g_usb_state_reg = 0;
        }
    }
}

/**
 * @brief       LED测试
 * @param       无
 * @retval      无
 */
void led_tset()
{
    printf("/******************************* 2，LED测试 *******************************/\r\n");
    printf("/********************* 注意：观察LED0和LED1是否闪烁！*********************/\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    while (1)
    {
        LED0_TOGGLE();                          /* LED0翻转 */
        LED1_TOGGLE();                          /* LED1翻转 */
        delay_ms(50);
        if (key_scan(0) == KEY1_PRES) break;    /* 当按下按键KEY1就退出 */
    }  
}

/**
 * @brief       蜂鸣器测试
 * @param       无
 * @retval      无
 */
void beep_test(void)
{
    printf("/******************************* 3，蜂鸣器测试 *******************************/\r\n");
    printf("/********************* 注意：听下蜂鸣器是否鸣叫！*********************/\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    beep_init();
    while (1)
    {  
        BEEP(0);                                /* 关闭蜂鸣器 */
        delay_ms(50);
        BEEP(1);                                /* 打开蜂鸣器 */
        delay_ms(50);
        if (key_scan(0) == KEY1_PRES) break;    /* 当按下按键KEY1就退出 */
    }   
    BEEP(0);                                    /* 关闭蜂鸣器 */
}

/**
 * @brief       按键测试
 * @param       无
 * @retval      无
 */
void key_test(void)
{
    printf("/******************************* 4，按键测试 *******************************/\r\n");
    printf("/*********************按下KEY0、KEY1、KEY_UP即可退出！*********************/\r\n"); 
    uint8_t key = 0;
    uint8_t status = 0;
    
    while(1)
    {
        key = key_scan(0);
        if (key)
        {
            status |= (1<<(key-1));
            switch(key)
            {
                case KEY0_PRES: printf("按键KEY0被按下\r\n");    break;
                case KEY1_PRES: printf("按键KEY1被按下\r\n");    break;
                case WKUP_PRES: printf("按键KEY_UP被按下\r\n");  break;
            }
        }
        if (status == 0x07) 
        {
            printf("按键全部按下，正常退出！\r\n");
            delay_ms(100);
            return;
        }
    }
}

/**
 * @brief       红外遥控测试
 * @param       无
 * @retval      无
 */
void remote_test(void)
{
    printf("\r\n/******************************* 5，红外遥控器测试 *******************************/\r\n");
    printf("/********* 注意：按下红外遥控器，串口输出对应键值，当按下Power将退出测试 *********/\r\n");
    printf("/****************************** NEXT: POWER ******************************/\r\n\r\n");
    uint8_t key = 0 , old_key = 0;
    char *str = 0;
    remote_init();
    while(1)
    {   
        /* 测试红外 */
        key = remote_scan();
        switch (key)
        {
            case 0:
                str = "ERROR";
                break;

            case 69:
                str = "POWER";
                break;

            case 70:
                str = "UP";
                break;

            case 64:
                str = "PLAY";
                break;

            case 71:
                str = "ALIENTEK";
                break;

            case 67:
                str = "RIGHT";
                break;

            case 68:
                str = "LEFT";
                break;

            case 7:
                str = "VOL-";
                break;

            case 21:
                str = "DOWN";
                break;

            case 9:
                str = "VOL+";
                break;

            case 22:
                str = "1";
                break;

            case 25:
                str = "2";
                break;

            case 13:
                str = "3";
                break;

            case 12:
                str = "4";
                break;

            case 24:
                str = "5";
                break;

            case 94:
                str = "6";
                break;

            case 8:
                str = "7";
                break;

            case 28:
                str = "8";
                break;

            case 90:
                str = "9";
                break;

            case 66:
                str = "0";
                break;

            case 74:
                str = "DELETE";
                break;
        }
        if(old_key != key)
        {
            printf("%s\r\n",str);
            old_key = key;
        }
        if (key == 69) break;                       /* 当按下电容触摸按键就退出 */
    }

}


/**
 * @brief       光敏测试
 * @param       无
 * @retval      无
 */
void lsens_test(void)
{
    uint8_t i = 0;
    printf("\r\n/******************************* 6，光敏测试 *******************************/\r\n");
    printf("/*** 注意：可以遮挡LS1，并观察光线强度值变化（0~100），光线越亮，值越大 ***/\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    lsens_init();                                                   /* 初始化光敏传感器 */

    while(1)
    {
        if(i++ > 100)
        {
            printf("光线强度值：%d\r\n",lsens_get_val());
            i = 0;
        }
        LED0_TOGGLE();
        delay_ms(10);
        if (key_scan(0) == KEY1_PRES) break;    /* 当按下按键KEY1就退出 */
    }
}

/**
 * @brief       ADC测试
 * @param       无
 * @retval      无
 */
void adc_tset()
{
    printf("\r\n/******************************* 7，ADC测试 *******************************/\r\n");
    printf("/ 注意：请接上P7处的跳线帽，将ADC与RV1进行短接，然后调节RV1观看ADC读数变化！！ /\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    uint16_t adcx,t = 0;
    float temp;
    adc_init();
    while (1)
    {
        if(t++ > 100)
        {
            t = 0;
            adcx = adc_get_result_average(ADC_ADCX_CHY, 10);    /* 获取通道14的转换值，10次取平均 */
            printf("ADC原始值：%d\r\n",adcx);
            temp = (float)adcx * (3.3 / 4096);                  /* 获取计算后的带小数的实际电压值，比如3.1111 */
            printf("ADC电压值：%.2fV\r\n",temp);
        }
        delay_ms(10);
        if (key_scan(0) == KEY1_PRES) break;                    /* 当按下按键KEY1就退出 */
    }  
}

/**
 * @brief       SD卡测试
 * @param       无
 * @retval      无
 */
void sd_test(void)
{
    printf("/*******************************  8，SD卡测试 *******************************/\r\n");
    printf("/**** 注意：插上SD卡，并显示SD卡容量大小，如果没有SD卡，直接按KEY1跳过！****/\r\n");
    printf("/****************************** NEXT: KEY1 ******************************/\r\n\r\n");
    uint8_t t = 0;
    while(sd_init())    /* 检测不到SD卡 */
    {
        if(t++ > 100)
        {
            t = 0;
            printf("SD卡初始化失败!,请检查下是否有插卡！！\r\n");
        }
        delay_ms(10);
        if (key_scan(0) == KEY1_PRES) return;               /* 当按下按键KEY1就退出 */
    }
    printf("SD卡初始化成功\r\n");
    printf("SD容量大小为：%lldMB\r\n",SD_TOTAL_SIZE_MB(&g_sdcard_handler));
    while(1)
    {
        if (key_scan(0) == KEY1_PRES) break;                /* 当按下按键KEY1就退出 */
    }  
}
/**
 * @brief       发货测试流程
 * @param       无
 * @retval      无
 */
void test_system(void)
{
    printf("\r\n/******************************* 无屏幕验收流程 *******************************/\r\n");
    
    at24cxx_init();
    while ( at24cxx_check() )                   /* 检查24c02是否正常，需要读取24c02的一个标志判断是否进行过出厂测试 */
    {
        printf("24C02 ERROR!\r\n");
        delay_ms(100);
    }   
    
    /* usb连接测试 */
    usb_test();
    
    /* LED测试 */
    led_tset();
    
    /* 蜂鸣器测试 */
    beep_test();
    
    /* 按键测试 */
    key_test();
    
    /* 红外遥控器测试 */
    remote_test();
    
    /* 光敏测试 */
    lsens_test();

    /* ADC测试 */
    adc_tset();
    
    /* SD卡测试 */ 
    sd_test();
    
    printf("\r\n/******************************* 测试完成 *******************************/\r\n");
}



