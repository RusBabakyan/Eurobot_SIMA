/*
 * ArduinoMath.h
 *
 *  Created on: Nov 28, 2023
 *      Author: Ruslan
 */

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "stm32f1xx_hal.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//#define sign(x) (x>=0)?1:-1

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

void delayMicro(uint16_t microseconds);

GPIO_PinState logic(bool x);

#ifdef __cplusplus
}
#endif

