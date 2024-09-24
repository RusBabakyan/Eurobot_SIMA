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
#include "VL53L0X.h"

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

#define XSHUT_1_Pin GPIO_PIN_13
#define XSHUT_1_GPIO_Port GPIOC
#define Power_Pin GPIO_PIN_14
#define Power_GPIO_Port GPIOC
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

// robot 1 - 21
// robot 2 - 28
// robot 3 - 28

//#define GEAR_RATIO 21
#define GEAR_RATIO 28


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
	void sensor_init();
	void update_distance();
	/* USER CODE END PFP */

	/* Private variables ---------------------------------------------------------*/
	/* USER CODE BEGIN PV */

	float speed_L = 0;
	float speed_R = 0;

	uint16_t distance;
	bool stopflag = 0;

	/* USER CODE BEGIN PV */

private:
	/* Private function prototypes -----------------------------------------------*/
	/* USER CODE BEGIN PFP */

	/* USER CODE END PFP */

	/* Private variables ---------------------------------------------------------*/
	/* USER CODE BEGIN PV */



	int16_t speed_min = PWM_pulse*0.1 + 10;
	int16_t speed_max = PWM_pulse*0.9 - 10;

	int32_t ticks_L = 0;
	int32_t ticks_R = 0;

	int32_t error_L = 0;
	int32_t error_R = 0;

	uint32_t time = 0;
	uint32_t dif_time = 0;

//	int32_t coeff = 44000;
	float Radius = 32;
	float Lenght = 108;
//	float ticks_per_rev = 44000;
	uint32_t gear = 21;
	uint32_t ticks = 1024;
	float ticks_per_rev = GEAR_RATIO * ticks * 2;
	float dist_per_rev  = M_PI * 2 * Radius;
	float th_diff;
	float d;
	float d_l;
	float d_r;

//	uint16_t Step = 1024;

//  sensor:

	statInfo_t_VL53L0X distanceStr1;
	VL53L0X dev1;
	bool sensor_ready;

	/* USER CODE BEGIN PV */
};

#ifdef __cplusplus
}
#endif

#endif /* INC_SIMACLASS_H_ */
