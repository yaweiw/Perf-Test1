#ifndef __DHT11_OPERATION_H_
#define __DHT11_OPERATION_H_

#include "common.h"
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

// Set GPIO Direction
#define IOT_DHT11_IO_IN(P) MicoGpioInitialize((mico_gpio_t)P, INPUT_PULL_UP);
#define IOT_DHT11_IO_OUT(P) MicoGpioInitialize((mico_gpio_t)P, OUTPUT_PUSH_PULL);

// Set Data output state
#define IOT_DHT11_DATA_Clr(P) MicoGpioOutputLow(P)
#define IOT_DHT11_DATA_Set(P) MicoGpioOutputHigh(P)

// get DATA input state
#define IOT_DHT11_DQ_IN(P) MicoGpioInputGet(P)

uint8_t IOT_DHT11_Read_Data(mico_gpio_t pin, uint8_t *temperature, uint8_t *humidity);
uint8_t IOT_DHT11_Init(mico_gpio_t pin);

#ifdef __cplusplus
}
#endif

#endif /* __DHT11_OPERATION_H_ */