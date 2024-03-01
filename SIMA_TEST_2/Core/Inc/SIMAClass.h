/*
 * SIMAClass.h
 *
 *  Created on: Feb 2, 2024
 *      Author: Ruslan
 */

#ifndef INC_SIMACLASS_H_
#define INC_SIMACLASS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <math.h>
#include "ArduinoMath.h"
#include "Terminal.h"

//#define SERVO_Pin GPIO_PIN_0
//#define SERVO_GPIO_Port GPIOA
#define DIR_L_Pin GPIO_PIN_3
#define DIR_L_GPIO_Port GPIOA
#define STP_L_Pin GPIO_PIN_4
#define STP_L_GPIO_Port GPIOA
#define DIR_R_Pin GPIO_PIN_5
#define DIR_R_GPIO_Port GPIOA
#define STP_R_Pin GPIO_PIN_6
#define STP_R_GPIO_Port GPIOA
//#define UART_TX_Pin GPIO_PIN_10
//#define UART_TX_GPIO_Port GPIOB
//#define UART_RX_Pin GPIO_PIN_11
//#define UART_RX_GPIO_Port GPIOB
//#define PWM_L_Pin GPIO_PIN_8
//#define PWM_L_GPIO_Port GPIOA
//#define PWM_R_Pin GPIO_PIN_9
//#define PWM_R_GPIO_Port GPIOA
//#define ENC_LA_Pin GPIO_PIN_4
//#define ENC_LA_GPIO_Port GPIOB
//#define ENC_LB_Pin GPIO_PIN_5
//#define ENC_LB_GPIO_Port GPIOB
//#define ENC_RA_Pin GPIO_PIN_6
//#define ENC_RA_GPIO_Port GPIOB
//#define ENC_RB_Pin GPIO_PIN_7
//#define ENC_RB_GPIO_Port GPIOB



#define PWM_pulse 16383
#define SERVO_MIN 25
#define SERVO_MAX 75
#define WHEELS_Rad
#define WHEELS_Len



class SIMA_Class {
public:
	/* Class constructor -----------------------------------------------*/
//	SIMA_Class();
	/* Public function prototypes -----------------------------------------------*/
	/* USER CODE BEGIN PFP */
	void set_wheels_speed(int16_t speed_L, int16_t speed_R, bool antistop_flag);
	void update_ticks();
	void update_position();
	void servo_write(uint8_t angle);
	/* USER CODE END PFP */



private:
	/* Private function prototypes -----------------------------------------------*/
	/* USER CODE BEGIN PFP */

	/* USER CODE END PFP */

	/* Private variables ---------------------------------------------------------*/
	/* USER CODE BEGIN PV */

	bool stopflag = 0;

	int16_t speed_min = PWM_pulse*0.1 + 10;
	int16_t speed_max = PWM_pulse*0.9 - 10;

	int32_t ticks_L = 0;
	int32_t ticks_R = 0;

	int32_t error_L = 0;
	int32_t error_R = 0;

	int32_t coeff = 44000;
	float Radius = 32;
	float Lenght = 108;
	uint16_t Step = 1024;

	/* USER CODE BEGIN PV */
};

#ifdef __cplusplus
}
#endif

#endif /* INC_SIMACLASS_H_ */
