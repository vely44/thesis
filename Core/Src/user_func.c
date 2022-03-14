/*
 * user_func.c
 *
 *  Created on: Feb 17, 2022
 *      Author: vely44
 */
/*
 * user_func.c
 *
 *  Created on: Feb 7, 2022
 *      Author: The Laptop
 */
/*
 * user_func.c
 *
 *  Created on: Feb 4, 2022
 *      Author: vely44
 */
#include <string.h>
#include <stdio.h>
#include "user_func.h"
#include "user_adc.h"
#include "main.h"

uint8_t data[20];
//system
void system_init(void){
	// Initialize all components
	//HAL
	HAL_Init();
	//Clock
	SystemClock_Config();
	// GPIO
	MX_GPIO_Init();
	//UART
	MX_USART2_UART_Init();
	//ADC
	//MX_ADC_Init();
	//Timer
	//MX_TIM3_Init();
	//UART Interrupt is ON
	HAL_UART_Receive_IT(&huart2, data , 1);

	debugtool("#sys# system started__________________________________________________");


}

void UART_reset(void)
{

	//UART
	MX_USART2_UART_Init();
	//UART Interrupt is OFF
	//HAL_UART_Receive_IT(&huart2, data , 1);

	debugtool("#sys# UART Restarted without Interrupt _______________________________");


}

void UART_set(void)
{

	//UART
	//MX_USART2_UART_Init();
	//UART Interrupt is ON
	HAL_UART_Receive_IT(&huart2, data , 1);

	debugtool("#sys# UART Restarted with Interrupt _______________________________");


}

void one_measure(void)
{
	uint16_t raw_data;
	float voltage;
	char message1[10];
	char message2[10];

	HAL_ADC_PollForConversion(&hadc, 1000);
	raw_data = HAL_ADC_GetValue(&hadc);
	voltage = raw_data;
	voltage = (voltage/4095*3.3)-0.04;//small error correction
	sprintf(message2, "%hu\r\n", voltage);
	//begin uart transmission
	HAL_UART_Transmit(&huart2, (uint8_t*)message2, strlen(message2), HAL_MAX_DELAY);
	return raw_data;
}

int one_measureV(void)
{
	uint16_t raw_data;
	float voltage;
	char message1[10];
	char message2[10];

	HAL_ADC_PollForConversion(&hadc, 1000);
	raw_data = HAL_ADC_GetValue(&hadc);
	voltage = raw_data;
	voltage = (voltage/4095*3.3)-0.04;//small error correction

	//begin uart transmission
	debugtool("value here");
	sprintf(message1, "%f\r\n", voltage);
	sprintf(message2, "%hu\r\n", raw_data);
	HAL_UART_Transmit(&huart2, (uint8_t*)message2, strlen(message2), HAL_MAX_DELAY);
	debugtool(message1);
	HAL_Delay(500);
	blinkLED(5);
	return raw_data;
}


//Low-Power/Sleep
void lowpower(void){

	HAL_PWREx_EnableUltraLowPower();
	debugtool("#sys# low power is on");
}

void lowpower_off(void){

	HAL_PWREx_DisableUltraLowPower();
	debugtool("#sys# low power is off");
}

void goto_sleep(void){

	//going to sleep
	//Suspend the clock for a while
	HAL_SuspendTick();
	//sleep mode
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	//done with sleeping, now we wake up
	debugtool("#sys# going in sleep mode");
	HAL_ResumeTick();

}

//timer
void start_timer(void){
	// Turn on TIM3
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	debugtool("#sys# timer started");

}



void stop_timer(void){
	//turn off TIM3
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
	debugtool("#sys# timer stopped");
}

//debug
//blink
void blinkLED(int x){

	int speed;
	speed = 250;
	if(x>50)
		speed=50;
	for (int i=0;i<x;i++)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(speed);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(speed);

	}
}
void debugtool(char message4[100]){
//send data on uart
	HAL_UART_Transmit(&huart2, (uint8_t*)message4, strlen(message4), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, (uint8_t*)"\n\r", strlen("\n\r"), HAL_MAX_DELAY);

}

uint8_t read_uart(void)
{
	uint8_t  UART1_rxBuffer[1]={0};

	while(HAL_UART_Receive(&huart2, UART1_rxBuffer, 1, HAL_MAX_DELAY)!=HAL_OK)
	{
		debugtool("#sys# input was processed");
		break;
	}
	debugtool("#sys#this is your input: ");
	HAL_UART_Transmit(&huart2, (uint8_t*)UART1_rxBuffer , 1 , HAL_MAX_DELAY);


	return UART1_rxBuffer[0];
}




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	  HAL_UART_Receive_IT(&huart2, data , 1);
}
