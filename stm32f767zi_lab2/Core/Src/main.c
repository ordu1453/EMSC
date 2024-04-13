/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "keyboard_lib.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


// Порты и номера контактов для Семисегментного индикатора младшего разряда

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  int key; // Кнопка клавиатуры
  int num1 = -1; // Первый аргумент
  int num2 = -1; // Второй аргумент
  int first;
  int second;
  int result;
  reset_key_pins();
  off_seg();
//  1 and 2
//  Вычитание второго
//  операнда из первого
//  Результат в виде двузначного десятичного числа. Если результат
//  отрицательный, то включить красный светодиод.

  while (1)
  {

	  while(!HAL_GPIO_ReadPin(USER_Button_GPIO_Port,USER_Button_Pin) || num1 == -1){
	  key = read_key();
	  if(key!=-1){ // Если кнопка нажата
	  num1 = key;
	   // Выводим число на старшем разряде
	   out_num_high(num1);
	   // Задержка, чтобы не считывать значения во время дребезга
	   HAL_Delay(300);
	  }
	  }
	  while(!HAL_GPIO_ReadPin(USER_Button_GPIO_Port,USER_Button_Pin) || num2 == -1){
	  key = read_key();
	  if(key!=-1){ // Если кнопка нажата
	  num2 = key;
	   out_num_low(num2);
	   HAL_Delay(300);
	  }
	  }
	  // Ждём нажатия кнопки для вывода результата
	  while(!HAL_GPIO_ReadPin(USER_Button_GPIO_Port,USER_Button_Pin)){}
	  off_seg();
	  HAL_Delay(300);
	  first = num1*10+num2;
	  num1 = -1; // Первый аргумент
	  num2 = -1; // Второй аргумент
	  while(!HAL_GPIO_ReadPin(USER_Button_GPIO_Port,USER_Button_Pin) || num1 == -1){

	  key = read_key();
	  if(key!=-1){ // Если кнопка нажата
	  num1 = key;
	   // Выводим число на старшем разряде
	   out_num_high(num1);
	   // Задержка, чтобы не считывать значения во время дребезга
	   HAL_Delay(300);
	  }
	  }
	  while(!HAL_GPIO_ReadPin(USER_Button_GPIO_Port,USER_Button_Pin) || num2 == -1){
	  key = read_key();
	  if(key!=-1){ // Если кнопка нажата
	  num2 = key;
	   out_num_low(num2);
	   HAL_Delay(300);
	  }
	  }
	  // Ждём нажатия кнопки для вывода результата
	  while(!HAL_GPIO_ReadPin(USER_Button_GPIO_Port,USER_Button_Pin)){}
	  off_seg();
	  second = num1*10+num2;
	  num1 = -1; // Первый аргумент
	  num2 = -1; // Второй аргумент

	  result = second - first;
	  if (result < 0){
		  result *= -1;
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
	  }
	  // Выводим результат, используя оба разряда
	  out_num_both(result);

	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }

  /* USER CODE END 3 */


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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LowSeg_B_Pin|HighSeg_DP_Pin|LowSeg_DP_Pin|LowSeg_C_Pin
                          |LowSeg_F_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, HighSeg_B_Pin|HighSeg_C_Pin|HighSeg_D_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LowSeg_A_GPIO_Port, LowSeg_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LowSeg_G_Pin|LowSeg_D_Pin|LowSeg_E_Pin|LED3_Pin
                          |LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, HighSeg_F_Pin|HighSeg_E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, HighSeg_A_Pin|HighSeg_G_Pin|Keyboard_col1_Pin|Keyboard_col2_Pin
                          |Keyboard_col3_Pin|Keyboard_col4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LowSeg_B_Pin HighSeg_DP_Pin LowSeg_DP_Pin LowSeg_C_Pin
                           LowSeg_F_Pin */
  GPIO_InitStruct.Pin = LowSeg_B_Pin|HighSeg_DP_Pin|LowSeg_DP_Pin|LowSeg_C_Pin
                          |LowSeg_F_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USER_Button_Pin Keyboard_row1_Pin Keyboard_row2_Pin Keyboard_row3_Pin
                           Keyboard_row4_Pin */
  GPIO_InitStruct.Pin = USER_Button_Pin|Keyboard_row1_Pin|Keyboard_row2_Pin|Keyboard_row3_Pin
                          |Keyboard_row4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : HighSeg_B_Pin HighSeg_C_Pin HighSeg_D_Pin */
  GPIO_InitStruct.Pin = HighSeg_B_Pin|HighSeg_C_Pin|HighSeg_D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : LowSeg_A_Pin */
  GPIO_InitStruct.Pin = LowSeg_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LowSeg_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LowSeg_G_Pin LowSeg_D_Pin LowSeg_E_Pin LED3_Pin
                           LED2_Pin */
  GPIO_InitStruct.Pin = LowSeg_G_Pin|LowSeg_D_Pin|LowSeg_E_Pin|LED3_Pin
                          |LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : HighSeg_F_Pin HighSeg_E_Pin */
  GPIO_InitStruct.Pin = HighSeg_F_Pin|HighSeg_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : HighSeg_A_Pin HighSeg_G_Pin Keyboard_col1_Pin Keyboard_col2_Pin
                           Keyboard_col3_Pin Keyboard_col4_Pin */
  GPIO_InitStruct.Pin = HighSeg_A_Pin|HighSeg_G_Pin|Keyboard_col1_Pin|Keyboard_col2_Pin
                          |Keyboard_col3_Pin|Keyboard_col4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
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
