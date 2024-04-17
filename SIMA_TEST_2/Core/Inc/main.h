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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define XSHUT_1_Pin GPIO_PIN_13
#define XSHUT_1_GPIO_Port GPIOC
#define SERVO_Pin GPIO_PIN_0
#define SERVO_GPIO_Port GPIOA
#define DIR_L_Pin GPIO_PIN_3
#define DIR_L_GPIO_Port GPIOA
#define STP_L_Pin GPIO_PIN_4
#define STP_L_GPIO_Port GPIOA
#define DIR_R_Pin GPIO_PIN_5
#define DIR_R_GPIO_Port GPIOA
#define STP_R_Pin GPIO_PIN_6
#define STP_R_GPIO_Port GPIOA
#define UART_TX_Pin GPIO_PIN_10
#define UART_TX_GPIO_Port GPIOB
#define UART_RX_Pin GPIO_PIN_11
#define UART_RX_GPIO_Port GPIOB
#define PWM_L_Pin GPIO_PIN_8
#define PWM_L_GPIO_Port GPIOA
#define PWM_R_Pin GPIO_PIN_9
#define PWM_R_GPIO_Port GPIOA
#define ENC_LA_Pin GPIO_PIN_4
#define ENC_LA_GPIO_Port GPIOB
#define ENC_LB_Pin GPIO_PIN_5
#define ENC_LB_GPIO_Port GPIOB
#define ENC_RA_Pin GPIO_PIN_6
#define ENC_RA_GPIO_Port GPIOB
#define ENC_RB_Pin GPIO_PIN_7
#define ENC_RB_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
