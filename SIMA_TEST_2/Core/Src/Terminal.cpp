/*
 * Terminal.cpp
 *
 *  Created on: Feb 2, 2024
 *      Author: Ruslan
 */



#include "Terminal.h"


uint8_t str[10];
uint8_t buf[3];
uint8_t rsp[15];
uint8_t crc = 0;

uint8_t CMD_LNG_IN[Count] = {5, 0, 1, 0};
uint8_t CMD_LNG_OUT[Count] = {0, 5, 0, 12};

CMD_SET_SPEED msg_in;

extern SIMA_Class SIMA;


uint8_t calc_crc(uint8_t cmd, uint8_t* data, uint8_t data_length){
	crc = cmd;
	for (uint8_t i = 0; i < data_length; i++){
		crc += data[i];
	}
	return crc;
}

HAL_StatusTypeDef Send_response(uint8_t cmd, uint8_t* data){
	uint8_t data_length = CMD_LNG_OUT[cmd];
	if (data_length == 0){
		rsp[0] = cmd;
		rsp[data_length+1] = cmd;
	} else {
		rsp[0] = cmd;
		rsp[data_length+1] = calc_crc(cmd, data, data_length);
		memcpy((uint8_t*)&rsp+1, data, data_length);
	}
	return (HAL_UART_Transmit(&huart3, rsp, data_length+2, data_length+2));
}

void TERMINAL(uint8_t cmd, uint8_t* str){

	switch (cmd){
	case SET_SPEED:
		{
		memcpy((uint8_t*)&msg_in, str, CMD_LNG_IN[cmd]);
		Send_response(SET_SPEED, rsp);
//		move_SIMA(MESSAGE, 0);
		SIMA.set_wheels_speed(msg_in.speed_L, msg_in.speed_R, msg_in.antistop_flag);
		}
		break;
	case PING:
		{
//		memcpy((uint8_t*)&msg_in, str, CMD_LNG_IN[cmd]);
		RSP_PING msg_out;
		msg_out.angle_L = 21;
		msg_out.angle_R = 14;
		msg_out.stop_flag = 0;
		Send_response(PING, (uint8_t*)&msg_out);
		}
		break;
	case GRIPPER:
		{
			uint8_t _angle = str[0];
			Send_response(GRIPPER, rsp);
			SIMA.servo_write(_angle);
		}
		break;
	case POSITION:
		{
		SIMA_POSITION msg_out;
		msg_out.ANGLE = 1234;
		msg_out.X = 5678;
		msg_out.Y = 9012;
		Send_response(POSITION, (uint8_t*)&msg_out);
		}
//
			break;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if (buf[0] == '!'){
		if (HAL_UART_Receive(huart, buf+1, 1, 1) == HAL_OK && buf[1] == '!'){
			if ((HAL_UART_Receive(huart, buf+2, 1, 1) == HAL_OK) && (buf[2] < Count)){

				uint8_t cmd = buf[2];
				uint8_t cmd_lenght = CMD_LNG_IN[cmd];
				if (HAL_UART_Receive(huart, str, cmd_lenght+1, cmd_lenght+1) == HAL_OK){

					uint8_t crc_recieved = str[cmd_lenght];
					crc = calc_crc(cmd, str, cmd_lenght);
					if (crc == crc_recieved){

						TERMINAL(cmd, str);
					}
				}
			}
		}
	}
	HAL_UART_Receive_IT (huart, buf, 1);
}
