/*
 * user_func.h
 *
 *  Created on: Feb 17, 2022
 *      Author: vely44
 */

#ifndef INC_USER_FUNC_H_
#define INC_USER_FUNC_H_

void system_init(void);

void UART_reset(void);

void UART_set(void);

void one_measure(void);

void tenset_measure(void);

void lowpower(void);

void lowpower_off(void);

void goto_sleep(void);

void wake_up(void);

void start_timer(void);

void stop_timer(void);

void blinkLED(int x);

void debugtool(char message4[100]);

//char read_uart(void);

uint8_t read_uart(void);

void read_uart_test(void);

#endif /* INC_USER_FUNC_H_ */
