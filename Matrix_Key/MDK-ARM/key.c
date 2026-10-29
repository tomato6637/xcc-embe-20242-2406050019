/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : 星火 STM32F407ZET6 矩阵键盘扫描 + LED 显示
  * 功能说明        : PE0~PE3 接矩阵键盘"行"，PE4~PE6 接矩阵键盘"列"(4行×3列=12键)
  *                   按下任意按键后，把键值(1~12)用二进制显示在 PF0~PF7 的 8 个 LED 上
  * 环境            : STM32CubeMX 生成工程 + Keil MDK + HAL 库
  * 使用方法        : 在 CubeMX 里按教程配好引脚并生成工程后，用本文件整体替换
  *                   Core/Src/main.c 即可（工程里不要开其他外设）
  ******************************************************************************
  * @attention
  * 1. 如果你的板子行列定义相反(PE0~PE2 行 / PE3~PE6 列)，只改下面 KEY_ROW_NUM、
  *    KEY_COL_NUM、RowPins、ColPins 四个地方即可，其他不用动
  * 2. LED 若是低电平点亮，把 LED_ON / LED_OFF 两个宏对调
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEY_ROW_NUM  4           /* 矩阵键盘行数 */
#define KEY_COL_NUM  3           /* 矩阵键盘列数 */
#define KEY_PORT     GPIOE       /* 键盘所在端口 */

#define LED_PORT     GPIOF       /* LED 所在端口 */
#define LED_ON       GPIO_PIN_SET   /* 高电平点亮；若你的板子是低电平点亮，改成 GPIO_PIN_RESET */
#define LED_OFF      GPIO_PIN_RESET /* 对应地，改成 GPIO_PIN_SET */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* 行引脚：PE0~PE3；列引脚：PE4~PE6
   若你的板子行列相反，把两个数组内容对调即可 */
static const uint16_t RowPins[KEY_ROW_NUM] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
static const uint16_t ColPins[KEY_COL_NUM] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
uint8_t Key_Scan(void);        /* 矩阵键盘扫描，返回 0=无按键，1~12=键值 */
void LED_ShowKey(uint8_t key); /* 把键值按二进制显示到 LED 上 */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  逐行扫描矩阵键盘（行列扫描法）
  * @retval 0 = 没有按键按下；1~12 = 按下的键值
  * @note   原理：行做输出、列做上拉输入。
  *         一次只把当前行拉低，其余行拉高，再读 3 个列引脚。
  *         若某一列读到低电平，说明"当前行×该列"交点处的键被按下。
  *         因为某一时刻只有一行是低电平，其他行不会干扰，所以能准确定位。
  */
uint8_t Key_Scan(void)
{
    uint8_t row, col;

    for (row = 0; row < KEY_ROW_NUM; row++)
    {
        /* 1. 先把 4 行全部拉高 */
        HAL_GPIO_WritePin(KEY_PORT,
                          GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
                          GPIO_PIN_SET);

        /* 2. 只把当前这一行拉低 */
        HAL_GPIO_WritePin(KEY_PORT, RowPins[row], GPIO_PIN_RESET);

        /* 3. 依次读 3 个列引脚，哪个变低就说明哪个键被按下 */
        for (col = 0; col < KEY_COL_NUM; col++)
        {
            if (HAL_GPIO_ReadPin(KEY_PORT, ColPins[col]) == GPIO_PIN_RESET)
            {
                /* 4. 延时 10ms 消抖后再次确认（防止机械抖动误触发） */
                HAL_Delay(10);
                if (HAL_GPIO_ReadPin(KEY_PORT, ColPins[col]) == GPIO_PIN_RESET)
                {
                    /* 5. 等待按键松开，避免按住时反复触发 */
                    while (HAL_GPIO_ReadPin(KEY_PORT, ColPins[col]) == GPIO_PIN_RESET)
                    {
                    }
                    /* 6. 返回键值：第 row 行第 col 列 → row*3+col+1（1~12） */
                    return (uint8_t)(row * KEY_COL_NUM + col + 1);
                }
            }
        }
    }
    return 0;   /* 没有按键按下 */
}

/**
  * @brief  把键值用二进制显示在 PF0~PF7 的 8 个 LED 上
  * @param  key: 键值 1~12
  * @note   例：键 5(0b00000101) → PF0、PF2 亮；键 10(0b00001010) → PF1、PF3 亮
  */
void LED_ShowKey(uint8_t key)
{
    uint16_t pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;

    /* 1. 先全部熄灭 */
    HAL_GPIO_WritePin(LED_PORT, pins, LED_OFF);

    /* 2. 键值对应的二进制位点亮（key & 0xFF 正好作为引脚掩码） */
    HAL_GPIO_WritePin(LED_PORT, (uint16_t)(key & 0xFF), LED_ON);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint8_t key = Key_Scan();      /* 扫描键盘 */
    if (key != 0)                  /* 有键按下 */
    {
      LED_ShowKey(key);            /* 点亮对应 LED */
    }
  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pins : PE0~PE3  键盘"行" → 推挽输出 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PE4~PE6  键盘"列" → 上拉输入 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PF0~PF7  LED → 推挽输出 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                      |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
