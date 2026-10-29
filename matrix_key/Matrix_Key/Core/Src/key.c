#include "key.h"

void Key_Init(void)
{
    HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN, GPIO_PIN_SET);
}

uint8_t Key_Scan(void)
{
    uint8_t row;

    for(row = 0; row < 4; row++)
    {
        HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN, GPIO_PIN_SET);

        switch(row)
        {
            case 0: HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW1_PIN, GPIO_PIN_RESET); break;
            case 1: HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW2_PIN, GPIO_PIN_RESET); break;
            case 2: HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW3_PIN, GPIO_PIN_RESET); break;
            case 3: HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW4_PIN, GPIO_PIN_RESET); break;
        }

        if(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL1_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(10);
            if(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL1_PIN) == GPIO_PIN_RESET)
            {
                while(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL1_PIN) == GPIO_PIN_RESET);
                return row * 4 + 1;
            }
        }

        if(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL2_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(10);
            if(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL2_PIN) == GPIO_PIN_RESET)
            {
                while(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL2_PIN) == GPIO_PIN_RESET);
                return row * 4 + 2;
            }
        }

        if(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL3_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(10);
            if(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL3_PIN) == GPIO_PIN_RESET)
            {
                while(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL3_PIN) == GPIO_PIN_RESET);
                return row * 4 + 3;
            }
        }

        if(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL4_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(10);
            if(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL4_PIN) == GPIO_PIN_RESET)
            {
                while(HAL_GPIO_ReadPin(COL_GPIO_PORT, COL4_PIN) == GPIO_PIN_RESET);
                return row * 4 + 4;
            }
        }
    }

    return 0;
}
