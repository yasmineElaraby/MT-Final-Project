/*
 * timer1.h
 *
 *  Created on: Nov 4, 2022
 *      Author: yasmi
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include<avr/io.h>
#include<avr/interrupt.h>
#include "std_types.h"
#include "common_macros.h"

/************** types definitions **********************/
typedef enum{
	NORMAL,COMPARE
}Timer1_Mode;

typedef enum{
	NO_CLOCK,F_CPU_1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXTERNAL_ON_RISING,EXTERNAL_ON_FALLING
}Timer1_Prescaler;

typedef struct {

	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.

} Timer1_ConfigType;

/**********************function prototypes***********************/
void Timer1_init(Timer1_ConfigType * Config_Ptr);

void Timer1_deInit(void);

void Timer1_setCallBack(void(*a_ptr)(void));

void Timer1_setCompareValue(uint16 value);

#endif /* TIMER1_H_ */
