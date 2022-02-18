/*
 * events.h
 *
 *  Created on: Feb 17, 2022
 *      Author: vely44
 */

#ifndef INC_EVENTS_H_
#define INC_EVENTS_H_

typedef enum {
   E_NO,
   E_SLEEP,
   E_WAKE_UP,
   E_1ST,
   E_2ND,
   E_RETURN,
   E_RETURN_TO_SLEEP, E_RETURN_MEASURE,
   E_RETURN_SETUP, E_RE_INIT// used for initialization of an event variable

} event_e;

#endif /* INC_EVENTS_H_ */
