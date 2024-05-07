/*
 * SIMAClass.cpp
 *
 *  Created on: Feb 2, 2024
 *      Author: Ruslan
 */

#include <SIMAClass.h>


//SIMA_Class::SIMA_Class() {
//	// TODO Auto-generated constructor stub
//
//}

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern SIMA_POSITION SIMA_POS;
extern I2C_HandleTypeDef hi2c1;



void SIMA_Class::set_wheels_speed(int16_t speed_L, int16_t speed_R, bool antistop_flag){
	bool dir_L = (speed_L >= 0)?1:0;
	bool stp_L = (speed_L == 0)?1:0;
	stp_L = (stp_L | (stopflag & !antistop_flag));
	speed_L = abs(speed_L);
	speed_L = constrain(speed_L, speed_min, speed_max);

	HAL_GPIO_WritePin(DIR_L_GPIO_Port, DIR_L_Pin, logic(dir_L));
	HAL_GPIO_WritePin(STP_L_GPIO_Port, STP_L_Pin, logic(stp_L));
	TIM1->CCR1 = abs(speed_L);

	bool dir_R = (speed_R >= 0)?1:0;
	bool stp_R = (speed_R == 0)?1:0;
	stp_R = (stp_R | (stopflag & !antistop_flag));
	speed_R = abs(speed_R);
	speed_R = constrain(speed_R, speed_min, speed_max);

	HAL_GPIO_WritePin(DIR_R_GPIO_Port, DIR_R_Pin, logic(!dir_R));
	HAL_GPIO_WritePin(STP_R_GPIO_Port, STP_R_Pin, logic(stp_R));
	TIM1->CCR2 = abs(speed_R);
}

void SIMA_Class::update_ticks(){
	static uint16_t prevCounter_L = 0;
	uint16_t currCounter_L = -__HAL_TIM_GET_COUNTER(&htim3);
	error_L = currCounter_L - prevCounter_L;

	if (error_L > (1 << 15))
		error_L = (int32_t) error_L - (1 << 16);
	else if (error_L < -1 * (1 << 15))
		error_L = (int32_t) error_L + (1 << 16);
	else {}
		ticks_L += (int32_t) error_L;
	prevCounter_L = currCounter_L;

	static uint16_t prevCounter_R = 0;
	uint16_t currCounter_R = __HAL_TIM_GET_COUNTER(&htim4);
	error_R = currCounter_R - prevCounter_R;

	if (error_R > (1 << 15))
		error_R = (int32_t) error_R - (1 << 16);
	else if (error_R < -1 * (1 << 15))
		error_R = (int32_t) error_R + (1 << 16);
	else {}
		ticks_R += (int32_t) error_R;
	prevCounter_R = currCounter_R;
}

void SIMA_Class::update_position(){
	static uint32_t update_timer = HAL_GetTick();
	uint32_t time = HAL_GetTick() - update_timer;
	if ((time) > 50){
		update_ticks();

		if ((error_R | error_L) == 0) return;

		speed_L = error_L * M_TWOPI / (ticks_per_rev * time);
		speed_R = error_R * M_TWOPI / (ticks_per_rev * time);

		d_l = error_L * dist_per_rev / ticks_per_rev;
		d_r = error_R * dist_per_rev / ticks_per_rev;
		th_diff = (d_r - d_l) / (Lenght);
		d = 	  (d_l + d_r) / 2;
		SIMA_POS.X += d * cos(SIMA_POS.ANGLE);
		SIMA_POS.Y += d * sin(SIMA_POS.ANGLE);
		SIMA_POS.ANGLE -= th_diff;
		SIMA_POS.ANGLE = SIMA_POS.ANGLE >  2*M_PI ? SIMA_POS.ANGLE - 2*M_PI : SIMA_POS.ANGLE;
		SIMA_POS.ANGLE = SIMA_POS.ANGLE < -2*M_PI ? SIMA_POS.ANGLE + 2*M_PI : SIMA_POS.ANGLE;
	}
}

void SIMA_Class::servo_write(uint8_t angle){
	angle = constrain(angle, 0, 90);
	angle = map(angle, 0, 90, SERVO_MIN, SERVO_MAX);
	TIM2->CCR1 = angle;
}

void SIMA_Class::sensor_init(){
	HAL_NVIC_DisableIRQ(USART3_IRQn);
	sensor_ready = true;
	dev1.g_i2cAddr = 0b01010010;
	dev1.g_ioTimeout = 0;
	dev1.g_isTimeout = 0;

	HAL_GPIO_WritePin(XSHUT_1_GPIO_Port, XSHUT_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Power_GPIO_Port, Power_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(XSHUT_1_GPIO_Port, XSHUT_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Power_GPIO_Port, Power_Pin, GPIO_PIN_SET);
	HAL_Delay(50);

	initVL53L0X(&dev1, 1, &hi2c1);
	HAL_Delay(10);
	setSignalRateLimit(&dev1, 200);
	HAL_Delay(10);
	setVcselPulsePeriod(&dev1, VcselPeriodPreRange, 12);
	HAL_Delay(10);
	setVcselPulsePeriod(&dev1, VcselPeriodFinalRange, 14);
	HAL_Delay(10);
//	setMeasurementTimingBudget(&dev1, 300 * 1000UL);
	setMeasurementTimingBudget(&dev1, (uint32_t)166000);

	HAL_Delay(100);

	startContinuous(&dev1,0);
	HAL_Delay(10);

	HAL_NVIC_EnableIRQ(USART3_IRQn);
}

void SIMA_Class::update_distance(){
	static uint32_t update_timer = HAL_GetTick();
	if (sensor_ready){
		if ((HAL_GetTick() - update_timer) > 50){
			distance = readRangeContinuousMillimeters(&dev1, &distanceStr1);
			stopflag = (distance < 150)?true:false;
		}
	}
}



