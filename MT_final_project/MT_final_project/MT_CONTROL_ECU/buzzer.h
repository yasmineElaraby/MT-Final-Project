/*
 * buzzer.h
 *
 *  Created on: Nov 5, 2022
 *      Author: yasmi
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"


#define BUZZER_BUTTON_PORT      PORTB_ID
#define BUZZER_BUTTON_PIN       PIN0_ID



/**********************function prototypes**************************/
void Buzzer_init();

void Buzzer_on();

void Buzzer_off();

#endif /* BUZZER_H_ */
