#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx_hal.h"

/* ================= 矩阵键盘引脚（PE0~PE6） =================
   4行×3列 = 12键：
   行(输出)：PE0~PE3
   列(输入，上拉)：PE4~PE6
   如果按下某键亮错灯/完全没反应，多半是行列对调了：
   把 KEY_ROW_NUM/KEY_COL_NUM 和 RowPin/ColPin 数组改一下即可 */
#define KEY_PORT        GPIOE
#define KEY_ROW_NUM     4
#define KEY_COL_NUM     3

#define ROW1_PIN        GPIO_PIN_0
#define ROW2_PIN        GPIO_PIN_1
#define ROW3_PIN        GPIO_PIN_2
#define ROW4_PIN        GPIO_PIN_3

#define COL1_PIN        GPIO_PIN_4
#define COL2_PIN        GPIO_PIN_5
#define COL3_PIN        GPIO_PIN_6

/* ================= LED 引脚（PF0~PF7） =================
   键 1~8 → LED1~8 一对一反馈；键 9~12 无对应 LED（保持全灭）
   若希望 12 个键都能显示，可把 Key_LED_Show 改成二进制显示 */
#define LED_PORT        GPIOF
#define LED1_PIN        GPIO_PIN_0
#define LED2_PIN        GPIO_PIN_1
#define LED3_PIN        GPIO_PIN_2
#define LED4_PIN        GPIO_PIN_3
#define LED5_PIN        GPIO_PIN_4
#define LED6_PIN        GPIO_PIN_5
#define LED7_PIN        GPIO_PIN_6
#define LED8_PIN        GPIO_PIN_7

/* LED 点亮电平：默认高电平点亮（引脚→LED→限流电阻→GND）
   如果你的板子是低电平点亮（LED 正极接 3.3V），把两个宏对调 */
#define LED_ON          GPIO_PIN_SET
#define LED_OFF         GPIO_PIN_RESET

void Key_Init(void);
uint8_t Key_Scan(void);
void Key_LED_Show(uint8_t key_num);

#endif /* __KEY_H */
