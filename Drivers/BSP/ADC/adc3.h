/**
 ****************************************************************************************************
 * @file        adc3.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2020-04-24
 * @brief       ADC3 驱动代码
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
 * V1.0 20200424
 * 第一次发布
 * V1.1 20200424
 * 1, 修改adc3_init, 添加对ADC3_CHY_GPIO相关的初始化
 * 2, 在头文件里面添加 ADC3_CHY 相关宏定义
 ****************************************************************************************************
 */

#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ADC及引脚 定义 */

#define ADC3_CHY_GPIO_PORT                  GPIOA
#define ADC3_CHY_GPIO_PIN                   GPIO_PIN_1 
#define ADC3_CHY_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  /* PA口时钟使能 */

#define ADC_ADCX                            ADC3 
#define ADC3_CHY                            ADC_CHANNEL_1                                /* 通道Y,  0 <= Y <= 17 */ 
#define ADC3_CHY_CLK_ENABLE()               do{ __HAL_RCC_ADC3_CLK_ENABLE(); }while(0)   /* ADC1 时钟使能 */


/******************************************************************************************/

void adc3_init(void);                               /* ADC3初始化 */
void adc3_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime);   /* ADC3通道设置 */
uint32_t adc3_get_result(uint32_t ch);               /* 获得某个通道值  */
uint32_t adc3_get_result_average(uint32_t ch, uint8_t times);/* 得到某个通道给定次数采样的平均值 */

#endif 















