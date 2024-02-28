/*
 * ArduinoMath.cpp
 *
 *  Created on: Nov 28, 2023
 *      Author: Ruslan
 */

#include "ArduinoMath.h"

#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void delayMicro(uint16_t microseconds){
    SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
    DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;   // запускаем счётчик

	uint32_t us_count_tic =  microseconds * (SystemCoreClock / 1000000); // получаем кол-во тактов за 1 мкс и умножаем на наше значение
	DWT->CYCCNT = 0U; // обнуляем счётчик
	while(DWT->CYCCNT < us_count_tic);
}

GPIO_PinState logic(bool x){
	if (x == 0) return GPIO_PIN_RESET;
	else return GPIO_PIN_SET;
}
