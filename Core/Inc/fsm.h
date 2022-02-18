/*
 * fsm.h
 *
 *  Created on: Feb 17, 2022
 *      Author: vely44
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "events.h"

event_e generateEvent(void);
void eventHandler(event_e evnt);

event_e CVMinitialise(void);
event_e CVMcheckCents(int coinValue);
event_e CVMcheckChange(void);
event_e check_input(void);
event_e check_input2(void);
event_e check_setup(void);
event_e check_freq(void);
char CNAinputCoin(void);
char KYBgetchar(void);

#endif /* INC_FSM_H_ */
