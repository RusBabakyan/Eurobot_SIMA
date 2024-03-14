/*
 * Terminal.h
 *
 *  Created on: Jan 20, 2024
 *      Author: Ruslan
 */

#ifndef INC_TERMINAL_H_
#define INC_TERMINAL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>


#include "ArduinoMath.h"
#include "Math.h"
#include "SIMAClass.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

enum CMD {SET_SPEED, PING, GRIPPER, POSITION, SET_POSITION, Count };

struct __attribute((packed)) CMD_SET_SPEED{
	int16_t speed_L;
	int16_t speed_R;
	bool antistop_flag;
};

struct __attribute((packed)) RSP_PING{
	int16_t angle_L;
	int16_t angle_R;
	bool stop_flag;
};

struct __attribute((packed)) SIMA_POSITION{
	float X;
	float Y;
	float ANGLE;
};

extern UART_HandleTypeDef huart3;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

uint8_t calc_crc(uint8_t cmd, uint8_t* data, uint8_t data_length);
HAL_StatusTypeDef Send_response(uint8_t cmd, uint8_t* data);
void TERMINAL(uint8_t cmd, uint8_t* str);
//
///* USER CODE END EFP */
//
///* Private defines -----------------------------------------------------------*/
//
///* USER CODE BEGIN Private defines */
//
///* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* INC_TERMINAL_H_ */
