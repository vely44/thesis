/*
 * user_adc.h
 *
 *  Created on: Feb 17, 2022
 *      Author: vely44
 */

#ifndef INC_USER_ADC_H_
#define INC_USER_ADC_H_
#include "stm32l0xx_hal.h"
#include "main.h"


void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
void MX_ADC_Init(void);
void MX_TIM3_Init(void);

#endif /* INC_USER_ADC_H_ */
