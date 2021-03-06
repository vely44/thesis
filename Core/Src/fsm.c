/*
 * fsm.c
 *
 *  Created on: Feb 17, 2022
 *      Author: vely44
 */
/*
 * fsm.c
 *
 *  Created on: Feb 9, 2022
 *      Author: The Laptop
 */

#include <string.h>
#include <stdio.h>
#include "fsm.h"
#include "user_func.h"
#include "user_adc.h"
#include "main.h"

typedef enum {
   S_NO,
   S_INITIALISED,
   S_SLEEPING_WAITING,
   S_WAIT_FOR_INPUT,
   S_MEASURE,
   S_SETUP
} state_e;
//Initial condition
state_e currentState = S_NO;
//event_e event = E_SLEEP;
//Global Variables
/*int insertedMoney = 0;
int priceCola = 0;
int change = 0;
int availableChange = 0; */

//-------------------------------------------------------------- generate events
// Because we do not buffer events in an event queue, we send in the
// current state only events that can be handled.
// Implementing buffering using a queue is outside the scope of this
// introduction to C programming.
event_e generateEvent(void)
{
   event_e evnt = E_SLEEP;
   switch(currentState)
   {
      case S_NO:
    	  debugtool("[FSM] #GENERATE EVENT# S_NO             ");
    	  debugtool("[FSM] #GENERATE EVENT# ////             ");
    	  debugtool("[FSM] #GENERATE EVENT# ////             ");
    	  debugtool("[FSM] #GENERATE EVENT# ////             ");
    	  debugtool("[FSM] #GENERATE EVENT# nothing here, just an empty state \r\n");
         break;
      case S_INITIALISED:
    	  debugtool("[FSM] #GENERATE EVENT# S_INITIALISED    ");
    	  debugtool("[FSM] #GENERATE EVENT# initialize variables");
    	  debugtool("[FSM] #GENERATE EVENT# send info on uart");
    	  debugtool("[FSM] #GENERATE EVENT# initializing communication and mandatory peripherals");
    	  debugtool("[FSM] #GENERATE EVENT# init uart            ");
    	  debugtool("[FSM] #GENERATE EVENT# blink 4            \r\n ");
         break;
      case S_SLEEPING_WAITING:
    	  debugtool("[FSM] #GENERATE EVENT# S_SLEEPING_WAITING");
    	  debugtool("[FSM] #GENERATE EVENT# turn on sleep             ");
    	  debugtool("[FSM] #GENERATE EVENT# wait for interrupt to wake up  \r\n");
    	  UART_set();
    	  goto_sleep();
         break;
      case S_WAIT_FOR_INPUT:
    	  UART_reset();
    	  debugtool("[FSM] #GENERATE EVENT# S_WAIT_FOR_INPUT ");
    	  debugtool("[FSM] #GENERATE EVENT# ////");
    	  debugtool("[FSM] #GENERATE EVENT# show Menu             \r\n ");
          evnt = check_input();
         break;
      case S_MEASURE:
    	  debugtool("[FSM] #GENERATE EVENT# S_MEASURE");
    	  debugtool("[FSM] #GENERATE EVENT# Turn on Timer (PWM)");

    	  //Init ADC TIM3
    	  MX_TIM3_Init();
    	  MX_ADC_Init();
    	  //Start ADC TIM3

    	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    	  debugtool("[FSM] #GENERATE EVENT# Measure ADC Voltage");
    	 int counter=10;
    	 int ccounter = counter;
    	 float average = 0;
    	 char message[10];
    	 while(counter)
    	 {
    		 HAL_ADC_Start(&hadc);
    		 average += one_measureV();
    		 HAL_ADC_Stop(&hadc);
    		 counter--;
    	 }
    	  debugtool("[FSM] #GENERATE EVENT# Send Answer on UART\r\n");
    	  average = average/ccounter;
    	  average = (average/4095*3.3)-0.04;
    	  sprintf(message, "%f\r\n", average);
    	  HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
    	  debugtool(message);
    	  //Stop ADC TIM3
    	  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);

         break;
      case S_SETUP:
    	  debugtool("[FSM] #GENERATE EVENT# S_SETUP");
    	  debugtool("[FSM] #GENERATE EVENT# Menu for tuning the FREQ and generate recommended scaling. ");
    	  debugtool("[FSM] #GENERATE EVENT# ////////////");
    	  debugtool("[FSM] #GENERATE EVENT# ////////////\r\n");
    	  evnt=check_input2();
         break;
   }

   return evnt;
}

void eventHandler(event_e event)
{
   state_e nextState = S_NO;

   switch(currentState)
   {
   case S_NO:
		  debugtool("[FSM]  $EVENT HANDLER$  E_NO  here ");
	      event = E_NO;
          nextState = S_INITIALISED;
          break;
   case S_INITIALISED:
		  debugtool("[FSM]  $EVENT HANDLER$  E_SLEEP  here ");
	      event = E_SLEEP;
          nextState = S_SLEEPING_WAITING;
          break;
   case S_SLEEPING_WAITING:
	   	  debugtool("[FSM]  $EVENT HANDLER$  E_WAKE_UP");
          event = E_WAKE_UP;
          nextState = S_WAIT_FOR_INPUT;
          break;
   case S_WAIT_FOR_INPUT:
	      switch (event)
          {
            case E_1ST:
            	debugtool("[FSM]  $EVENT HANDLER$  E_1ST on the user ");
                nextState = S_MEASURE;
                break;
            case E_2ND:
                 debugtool("[FSM]  $EVENT HANDLER$  E_2ND on the user ");
                 nextState = S_SETUP;
                 break;
            case E_RETURN:
            	debugtool("[FSM]  $EVENT HANDLER$  E_RETURN on the user ");
                nextState = S_WAIT_FOR_INPUT;
                break;
            default:
            	debugtool("[FSM]  $EVENT HANDLER$  default on the user ");
                nextState = S_WAIT_FOR_INPUT;
           }
          break;
	case S_SETUP:
	      	switch (event)
	        {
	            case E_RE_INIT:
	            	debugtool("[FSM]  $EVENT HANDLER$  E_RE_iNIT on the user ");
	            	nextState = S_INITIALISED;
	            	break;
	            case E_RETURN_MEASURE:
	            	debugtool("[FSM]  $EVENT HANDLER$  E_RETURN_MEASURE on the user ");
	            	nextState = S_MEASURE;
	            	break;
	            case E_RETURN_SETUP:
	            	debugtool("[FSM]  $EVENT HANDLER$  E_RETURN_SETUP on the user ");
	            	nextState = S_SETUP;
	            	break;
	            default:
	            	debugtool("[FSM]  $EVENT HANDLER$  default on the user ");
	            	nextState = S_SETUP;
	            }
	          break;
   case S_MEASURE:
          debugtool("[FSM]  $EVENT HANDLER$  E_RETURN_TO_SLEEP");
          event = E_RETURN_TO_SLEEP;
          nextState = S_SLEEPING_WAITING;
          break;
   }
   currentState = nextState;
}


event_e check_input(void)
{
  // char coin = '0';
   event_e event = E_SLEEP;
   debugtool("[USERFUNC] Select an option for the next stage.  ");
   debugtool("[USERFUNC] TYPE NOW: a-For Measuring / b-For Setup  ");




   switch(read_uart())
   {
      case 'a':
    	  event = E_1ST;
         break;
      case 'b':
         event = E_2ND;
         break;
      default:
    	  event = E_RETURN;
    	  break;
   }
   return event;
}

event_e check_input2(void)
{
  // char coin = '0';
   event_e event = E_WAKE_UP;
   debugtool("[USERFUNC] Select an option for the next stage.  ");
   debugtool("[USERFUNC] TYPE NOW: a-For ReInitialisation / b-For Measuring ");




   switch(read_uart())
   {
      case 'a':
    	  event = E_RE_INIT;
         break;
      case 'b':
         event = E_RETURN_MEASURE;
         break;
      default:
    	  event = E_RETURN_SETUP;
    	  break;
   }
   return event;
}

char KYBgetchar(void)
{
   char c = read_uart();
   return c;
}

