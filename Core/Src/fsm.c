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
    	  HAL_ADC_Start(&hadc);
    	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    	  debugtool("[FSM] #GENERATE EVENT# Measure ADC Voltage");
    	 int counter=10;
    	 while(counter)
    	 {
    		 one_measure();
    		 counter--;
    	 }
    	  debugtool("[FSM] #GENERATE EVENT# Send Answer on UART\r\n");

    	  //Stop ADC TIM3
    	  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
    	  HAL_ADC_Stop(&hadc);
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

//------------------------------------------------ CVM functions executed in FSM
/*
event_e CVMinitialise(void)
{
   // Initialise all subsystems (devices)
    debugtool("[FSM]init part");
    blinkLED(7);

   return E_SLEEP;
}
*/




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


/*
event_e check_setup(void)
{
	char coin = '0';
	event_e event = E_SLEEP;

   debugtool("[USERFUNC] Checking Setup ");
   blinkLED(2);

   coin = '2';
   //coin = CNAinputCoin();
   switch(coin)
   {
      case '1':
         event = E_SETFREQ;
         break;
      case '2':
         event = E_START_NEW_MEASURE;
         break;
      default:
    	  event = E_RETURN_SLEEP;
    	  break;
   }
   return event;
}
*/ /*
event_e check_freq(void)
{
	char coin = '0';
	int freq;
	event_e event = E_SLEEP;

   debugtool("[USERFUNC] Checking Frequency ");
   blinkLED(2);

   coin = '2';
   //coin = CNAinputCoin();
   switch(coin)
   {
      case '1':
         freq = 10;
    	  event = E_RETURN_SLEEP;
         break;
      case '2':
          freq = 50;
         event = E_RETURN_SLEEP;
         break;
      default:
          freq = 100;
    	  event = E_RETURN_SLEEP;
    	  break;
   }
   return event;
}

*/ /*
char CNAinputCoin(void)
{
   int coinIsOK = 0;
   char coin = '0';
   while (!coinIsOK)
   {
      coin = KYBgetchar();
      switch (coin)
      {
         case '1':
        	 debugtool("[FSM] case 1 ");
         case '2':
        	 debugtool("[FSM] and case 2");
            coinIsOK = 1;
            break;
         default:
            debugtool("[FSM] case 1 and case 2");
            blinkLED(2);
            break;
      }
   }
   return coin;
}
*/
char KYBgetchar(void)
{
   char c = read_uart();
   return c;
}

