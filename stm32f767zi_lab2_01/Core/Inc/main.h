/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LowSeg_B_Pin GPIO_PIN_2
#define LowSeg_B_GPIO_Port GPIOE
#define HighSeg_DP_Pin GPIO_PIN_3
#define HighSeg_DP_GPIO_Port GPIOE
#define USER_Button_Pin GPIO_PIN_13
#define USER_Button_GPIO_Port GPIOC
#define HighSeg_B_Pin GPIO_PIN_0
#define HighSeg_B_GPIO_Port GPIOF
#define HighSeg_C_Pin GPIO_PIN_7
#define HighSeg_C_GPIO_Port GPIOF
#define HighSeg_D_Pin GPIO_PIN_9
#define HighSeg_D_GPIO_Port GPIOF
#define LowSeg_A_Pin GPIO_PIN_0
#define LowSeg_A_GPIO_Port GPIOA
#define LowSeg_G_Pin GPIO_PIN_0
#define LowSeg_G_GPIO_Port GPIOB
#define HighSeg_F_Pin GPIO_PIN_0
#define HighSeg_F_GPIO_Port GPIOG
#define HighSeg_E_Pin GPIO_PIN_1
#define HighSeg_E_GPIO_Port GPIOG
#define LowSeg_DP_Pin GPIO_PIN_12
#define LowSeg_DP_GPIO_Port GPIOE
#define LowSeg_C_Pin GPIO_PIN_15
#define LowSeg_C_GPIO_Port GPIOE
#define LowSeg_D_Pin GPIO_PIN_10
#define LowSeg_D_GPIO_Port GPIOB
#define LowSeg_E_Pin GPIO_PIN_11
#define LowSeg_E_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOB
#define Keyboard_row1_Pin GPIO_PIN_8
#define Keyboard_row1_GPIO_Port GPIOC
#define Keyboard_row2_Pin GPIO_PIN_9
#define Keyboard_row2_GPIO_Port GPIOC
#define Keyboard_row3_Pin GPIO_PIN_10
#define Keyboard_row3_GPIO_Port GPIOC
#define Keyboard_row4_Pin GPIO_PIN_11
#define Keyboard_row4_GPIO_Port GPIOC
#define HighSeg_A_Pin GPIO_PIN_0
#define HighSeg_A_GPIO_Port GPIOD
#define HighSeg_G_Pin GPIO_PIN_1
#define HighSeg_G_GPIO_Port GPIOD
#define Keyboard_col1_Pin GPIO_PIN_4
#define Keyboard_col1_GPIO_Port GPIOD
#define Keyboard_col2_Pin GPIO_PIN_5
#define Keyboard_col2_GPIO_Port GPIOD
#define Keyboard_col3_Pin GPIO_PIN_6
#define Keyboard_col3_GPIO_Port GPIOD
#define Keyboard_col4_Pin GPIO_PIN_7
#define Keyboard_col4_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB
#define LowSeg_F_Pin GPIO_PIN_0
#define LowSeg_F_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
