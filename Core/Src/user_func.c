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
	MX_USART2_UART_Init();
	//UART Interrupt is ON
	HAL_UART_Receive_IT(&huart2, data , 1);

	debugtool("#sys# UART Restarted with Interrupt _______________________________");


}

void one_measure(void)
{
	uint16_t raw_data;
	float voltage;
	char message1[10];

	HAL_ADC_PollForConversion(&hadc, 300);
	raw_data = HAL_ADC_GetValue(&hadc);
	voltage = raw_data;
	voltage = (voltage/4095*3.3)-0.04;//small error correction

	//begin uart transmission
	debugtool("value here");
	sprintf(message1, "%hu\r\n", voltage);
	debugtool(message1);
	HAL_Delay(500);
	blinkLED(5);
	raw_data= 0;
}

//not used
void tenset_measure(void)
{
	uint16_t raw_data;
	float voltage;
	for(int i=0;i<=10;i++)
	{
		HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
		raw_data = HAL_ADC_GetValue(&hadc);
		voltage = raw_data;
		voltage = (voltage/4095*3.3)-0.04;//small error correction
		//toggle led on for conversion tracking

		//begin uart transmission
		debugtool("value here");
		blinkLED(1);
	}
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

void wake_up(void){


	debugtool("#sys# the device is back in normal mode");
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
	uint8_t  UART1_rxBuffer[1];

	while(HAL_UART_Receive(&huart2, UART1_rxBuffer, 1, HAL_MAX_DELAY)!=HAL_OK)
	{
		debugtool("#sys# input was processed");
		break;
	}
	debugtool("#sys#this is your input: ");
	HAL_UART_Transmit(&huart2, (uint8_t*)UART1_rxBuffer , 1 , HAL_MAX_DELAY);


	return UART1_rxBuffer[0];
}

//old test version
void read_uart_test(void)
{
	uint8_t UART1_rxBuffer[10] = {0};
	debugtool("MENU 1/2");
	while(HAL_UART_Receive(&huart2, UART1_rxBuffer,1,500)!=HAL_OK){
		debugtool("yes");
		break;
	}
	//HAL_UART_Receive(&huart2, UART1_rxBuffer,20,300);
	HAL_UART_Transmit(&huart2, UART1_rxBuffer , 1 , 300);
	HAL_UART_Transmit(&huart2, UART1_rxBuffer , 1 , 300);
	HAL_UART_Transmit(&huart2, UART1_rxBuffer , 1 , 300);
	HAL_UART_Transmit(&huart2, UART1_rxBuffer , 1 , 300);
	HAL_UART_Transmit(&huart2, UART1_rxBuffer , 1 , 300);
	debugtool("printed");
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	  HAL_UART_Receive_IT(&huart2, data , 1);
}
