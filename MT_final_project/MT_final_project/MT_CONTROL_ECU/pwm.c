/*********************************
 * pwm.c                         *
 *  Created on: Oct 6, 2022      *
 *      Author: yasmine          *
 *********************************/
#include "pwm.h"
#include <avr/io.h>

void PWM_Timer0_Start(uint8 duty_cycle){

	 /* counter mode  -> fast PWM / non-inverting
	 * f_prescaler   -> F_CPU/8
	 * compare value -> duty_cycle * (256 / 100)
	 * f_pwm         -> 500 Hz
	 * */

	/*  clear counter register */
	TCNT0 = 0;

	/* adjust top value according to (duty cycle) */
	OCR0 = duty_cycle *2.55; ;


	TCCR0  = (1<<WGM01) | (1<<WGM00) | (1<<COM01) | (1<<CS01);

    DDRB |= (1<<PB3);

}

