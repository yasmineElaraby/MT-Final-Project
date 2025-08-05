/*********************************
 * pwm.h                         *
 *  Created on: Oct 6, 2022      *
 *      Author: yasmine          *
 *********************************/

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

#define TIMER0_TOP_VALUE 255

/*****functions prototypes*****/
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
