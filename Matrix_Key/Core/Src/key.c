#include "key.h"

/* ===== K 编号映射表 =====
   扫描键号(1~12) → 丝印键号 K(1~12)，默认 1:1
   即：行1列1=K1, 行1列2=K2, ... 行4列2=K11, 行4列3=K12
   如果按下丝印 K1 亮的却是别的灯，说明接线顺序和丝印对不上，
   把 key_map[扫描键号] 改成实际丝印编号即可，例如：
   key_map[3]=1 表示"扫描到的第 3 个键，丝印其实是 K1" */
static const uint8_t key_map[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};

/* 行引脚表：PE0~PE3；列引脚表：PE4~PE6 */
static const uint16_t RowPin[KEY_ROW_NUM] = {ROW1_PIN, ROW2_PIN, ROW3_PIN, ROW4_PIN};
static const uint16_t ColPin[KEY_COL_NUM] = {COL1_PIN, COL2_PIN, COL3_PIN};
static const uint16_t RowAllPin = ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN;

void Key_Init(void)
{
    /* 上电先把 4 行全部拉高，避免误判有键按下 */
    HAL_GPIO_WritePin(KEY_PORT, RowAllPin, GPIO_PIN_SET);
}

/* 逐行扫描：行做输出，列做上拉输入。
   一次只把当前行拉低，其余行拉高，再读 3 个列引脚；
   哪一列变低，就是"当前行 × 该列"交点的键被按下 */
uint8_t Key_Scan(void)
{
    uint8_t row, col;

    for(row = 0; row < KEY_ROW_NUM; row++)
    {
        /* 1. 4 行全部拉高，再把当前行拉低 */
        HAL_GPIO_WritePin(KEY_PORT, RowAllPin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(KEY_PORT, RowPin[row], GPIO_PIN_RESET);

        /* 2. 依次读 3 列 */
        for(col = 0; col < KEY_COL_NUM; col++)
        {
            if(HAL_GPIO_ReadPin(KEY_PORT, ColPin[col]) == GPIO_PIN_RESET)
            {
                /* 3. 延时 10ms 消抖，再确认一次 */
                HAL_Delay(10);
                if(HAL_GPIO_ReadPin(KEY_PORT, ColPin[col]) == GPIO_PIN_RESET)
                {
                    /* 4. 等待松开，防止按住时反复触发 */
                    while(HAL_GPIO_ReadPin(KEY_PORT, ColPin[col]) == GPIO_PIN_RESET)
                    {
                    }
                    /* 5. 返回扫描键号 1~12 */
                    return (uint8_t)(row * KEY_COL_NUM + col + 1);
                }
            }
        }
    }

    return 0;   /* 无按键按下 */
}

/* 扫描键号(1~8) → 点亮对应 LED；K9~12 无对应 LED，保持全灭 */
void Key_LED_Show(uint8_t key_num)
{
    uint8_t k;
    uint16_t all_led = LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN
                     |LED5_PIN|LED6_PIN|LED7_PIN|LED8_PIN;

    /* 先全部熄灭 */
    HAL_GPIO_WritePin(LED_PORT, all_led, LED_OFF);

    if(key_num == 0 || key_num > 12)
    {
        return;
    }

    k = key_map[key_num];   /* 扫描键号 → 丝印 K 编号 */

    if(k == 0 || k > 8)
    {
        return;   /* K9~12 无对应 LED */
    }

    /* LED1~LED8 在 PF0~PF7 连续排列，LED1_PIN<<(k-1) 正好是第 k 个 LED */
    HAL_GPIO_WritePin(LED_PORT, (uint16_t)(LED1_PIN << (k - 1)), LED_ON);
}
