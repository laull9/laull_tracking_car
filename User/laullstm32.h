#ifndef _LAULLSTM32_
#define _LAULLSTM32_

#ifdef __cplusplus
extern "C"{

#else
#include "stdbool.h"
#endif

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/IIC/myiic.h"
#include "./MALLOC/malloc.h"
#include "./BSP/LSENS/lsens.h"
#include "./BSP/24CXX/24cxx.h"
#include "./BSP/REMOTE/remote.h"
#include "./MALLOC/malloc.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/TOUCH/touch.h"
#include "TEXT/text.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h"
#include "./TEXT/text.h"
#include "./PICTURE/piclib.h"

#ifdef __cplusplus
} // !extern "C"
#endif


#define LED0_ON() LED0(0)
#define LED1_ON() LED1(0)
#define LED0_OFF() LED0(1)
#define LED1_OFF() LED1(1)
#define BEEP_ON() BEEP(1)
#define BEEP_OFF() BEEP(0)

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t


//---------------激活gpio针脚---------------
//输入针脚名+数字+模式，如laull_gpio_init(A, 10, GPIO_MODE_AF_PP)初始化A10针脚
#define laull_gpio_init(GPIO_TYPE, GPIO_NUM, GPIO_MODE) do{   \
  __HAL_RCC_GPIO##GPIO_TYPE##_CLK_ENABLE();        \
  GPIO_InitTypeDef GPIO_InitStruct = {0};          \
  GPIO_InitStruct.Pin = GPIO_PIN_##GPIO_NUM;       \
  GPIO_InitStruct.Mode = GPIO_MODE;                \
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     \
  HAL_GPIO_Init(GPIO##GPIO_TYPE, &GPIO_InitStruct);\
}while (0);


//---------------激活tim通道---------------
//TIM_Handle               [TIM_HandleTypeDef]
//TIM_NUM, TIM_CHANNEL_NUM [数字],[数字] 表示TIM(X)的chennel_(Y)
//arr, psc                 [数字],[数字] 自动重装值, 时钟预分频数
//TIM_OCPOLARITY_XX        [仅TIM_OCPOLARITY_LOW 或 TIM_OCPOLARITY_HIGH] 高低电平
#define laull_tim_init(TIM_Handle, TIM_NUM, TIM_CHANNEL_NUM,                          \
                        arr, psc, TIM_OCPOLARITY_XX) do{                              \
  __HAL_RCC_TIM##TIM_NUM##_CLK_ENABLE();                                              \
  TIM_Handle.Instance = TIM##TIM_NUM;                                                 \
  TIM_Handle.Init.Prescaler = psc;                                                    \
  TIM_OC_InitTypeDef timx_oc_pwm  = {0};                                              \
  TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;                                   \
  TIM_Handle.Init.Period = arr;                                                       \
  HAL_TIM_PWM_Init(&TIM_Handle);                                                      \
  timx_oc_pwm.OCMode = TIM_OCMODE_PWM1;                                               \
  timx_oc_pwm.Pulse = arr / 2;                                                        \
  timx_oc_pwm.OCPolarity = TIM_OCPOLARITY_XX;                                         \
  HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &timx_oc_pwm, TIM_CHANNEL_##TIM_CHANNEL_NUM);\
  HAL_TIM_PWM_Start(&TIM_Handle, TIM_CHANNEL_##TIM_CHANNEL_NUM);                      \
}while(0);

 
void chinese_init();

bool scan_touched(u16 *x, u16 *y);

#ifdef __cplusplus
bool scan_touched(u16 &x, u16 &y);
#endif

void do_touch_adjust();
// 画粗线
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color);

#define ALL_INIT() do{\
	HAL_Init();                                \
	sys_stm32_clock_init(RCC_PLL_MUL9);        \
	delay_init(72);                            \
	usart_init(115200);                        \
	led_init();                                \
	key_init();                                \
	lcd_init();                                \
	norflash_init();                           \
	my_mem_init(SRAMIN);                       \
	/*sd_init();  */                               \
	beep_init();                               \
	remote_init();                             \
	tp_init();                                 \
	/*at24cxx_init();*/                            \
	/* piclib_init(); */           /*画图*/          \
	/*piclib_ai_draw_init()*/;    /* 初始化智能画图 */ \
}while(0);

void filesystem_init();

#ifdef __cplusplus
#include <vector>
// 获得一个路径下的所有文件路径，需要释放内存
// mode : 1-绝对路径 0-仅文件名
bool get_dir_all_files(const char* dirpath, std::vector<char *> &out_paths, u8 mode = 0);

// 释放文件路径内存
void free_filepaths(std::vector<char *> &out_paths);
#endif


#endif