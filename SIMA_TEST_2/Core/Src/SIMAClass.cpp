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

void SIMA_Class::set_wheels_speed(int16_t speed_L, int16_t speed_R, bool antistop_flag){
	bool dir_L = (speed_L >= 0)?1:0;
	bool stp_L = (speed_L == 0)?1:0;
	stp_L = (stp_L | stopflag) & antistop_flag;
	speed_L = abs(speed_L);
	speed_L = constrain(speed_L, speed_min, speed_max);

	HAL_GPIO_WritePin(DIR_L_GPIO_Port, DIR_L_Pin, logic(dir_L));
	HAL_GPIO_WritePin(STP_L_GPIO_Port, STP_L_Pin, logic(stp_L));
	TIM1->CCR1 = abs(speed_L);

	bool dir_R = (speed_R >= 0)?1:0;
	bool stp_R = (speed_R == 0)?1:0;
	stp_R = (stp_R | stopflag) & antistop_flag;
	speed_R = abs(speed_R);
	speed_R = constrain(speed_R, speed_min, speed_max);

	HAL_GPIO_WritePin(DIR_R_GPIO_Port, DIR_R_Pin, logic(!dir_R));
	HAL_GPIO_WritePin(STP_R_GPIO_Port, STP_R_Pin, logic(stp_R));
	TIM1->CCR2 = abs(speed_R);
}

void SIMA_Class::update_ticks(){
	static uint16_t prevCounter_L = 0;
	uint16_t currCounter_L = __HAL_TIM_GET_COUNTER(&htim3);
	error_L = currCounter_L - prevCounter_L;

	if (error_L > (1 << 15))
		ticks_L += (int32_t) error_L - (1 << 16);
	else if (error_L < -1 * (1 << 15))
		ticks_L += (int32_t) error_L + (1 << 16);
	else
		ticks_L += (int32_t) error_L;

	prevCounter_L = currCounter_L;

	static uint16_t prevCounter_R = 0;
	uint16_t currCounter_R = __HAL_TIM_GET_COUNTER(&htim4);
	error_R = currCounter_R - prevCounter_R;

	if (error_R > (1 << 15))
		ticks_R += (int32_t) error_R - (1 << 16);
	else if (error_R < -1 * (1 << 15))
		ticks_R += (int32_t) error_R + (1 << 16);
	else
		ticks_R += (int32_t) error_R;

	prevCounter_R = currCounter_R;
}

void SIMA_Class::update_position(){
	update_ticks();

	if (error_R == error_L){
		float R = (float)(Lenght/2)*(error_L+error_R)/(error_L-error_R);
		float Wdt = (float)(error_L-error_R)*coeff/Lenght;

		SIMA_POS.ANGLE += Wdt;

		float SIN_Wdt = sin(Wdt);
		float COS_Wdt = cos(Wdt);
		float ICC_X = SIMA_POS.X - R * sin(SIMA_POS.ANGLE);
		float ICC_Y = SIMA_POS.Y - R * cos(SIMA_POS.ANGLE);

		float X_ICC = SIMA_POS.X - ICC_X;
		float Y_ICC = SIMA_POS.Y - ICC_Y;

		SIMA_POS.X += COS_Wdt * X_ICC - SIN_Wdt * Y_ICC;
		SIMA_POS.Y += SIN_Wdt * X_ICC + COS_Wdt * Y_ICC;
	} else {
		float R = (float)(Lenght/2)*(error_L+error_R)/(error_L-error_R);
		float Wdt = (float)(error_L-error_R)*coeff/Lenght;

		SIMA_POS.ANGLE += Wdt;

		float SIN_Wdt = sin(Wdt);
		float COS_Wdt = cos(Wdt);
		float ICC_X = SIMA_POS.X - R * sin(SIMA_POS.ANGLE);
		float ICC_Y = SIMA_POS.Y - R * cos(SIMA_POS.ANGLE);

		float X_ICC = SIMA_POS.X - ICC_X;
		float Y_ICC = SIMA_POS.Y - ICC_Y;

		SIMA_POS.X += COS_Wdt * X_ICC - SIN_Wdt * Y_ICC;
		SIMA_POS.Y += SIN_Wdt * X_ICC + COS_Wdt * Y_ICC;
	}
}

void SIMA_Class::servo_write(uint8_t angle){
	angle = constrain(angle, 0, 90);
	angle = map(angle, 0, 90, SERVO_MIN, SERVO_MAX);
	TIM2->CCR1 = angle;
}



