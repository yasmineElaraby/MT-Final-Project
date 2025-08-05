/*
 * buzzer.c
 *
 *  Created on: Nov 5, 2022
 *      Author: yasmi
 */

#include "buzzer.h"
#include "gpio.h"
#include "common_macros.h"

/**************function definition************/
void Buzzer_init(){

	GPIO_setupPinDirection(BUZZER_BUTTON_PORT,BUZZER_BUTTON_PIN,PIN_OUTPUT);

	GPIO_writePin(BUZZER_BUTTON_PORT, BUZZER_BUTTON_PIN, LOGIC_LOW);
}

void Buzzer_on(){

	GPIO_writePin(BUZZER_BUTTON_PORT, BUZZER_BUTTON_PIN, LOGIC_HIGH);
}

void Buzzer_off(){

	GPIO_writePin(BUZZER_BUTTON_PORT, BUZZER_BUTTON_PIN, LOGIC_LOW);
}
