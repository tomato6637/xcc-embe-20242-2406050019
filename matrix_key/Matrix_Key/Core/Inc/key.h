#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx_hal.h"

/* 行线（输出）：PD0~PD3 */
#define ROW_GPIO_PORT     GPIOD
#define ROW1_PIN          GPIO_PIN_0
#define ROW2_PIN          GPIO_PIN_1
#define ROW3_PIN          GPIO_PIN_2
#define ROW4_PIN          GPIO_PIN_3

/* 列线（输入）：PD4~PD7 */
#define COL_GPIO_PORT     GPIOD
#define COL1_PIN          GPIO_PIN_4
#define COL2_PIN          GPIO_PIN_5
#define COL3_PIN          GPIO_PIN_6
#define COL4_PIN          GPIO_PIN_7

void Key_Init(void);
uint8_t Key_Scan(void);

#endif /* __KEY_H */
