/*
 * timer1.c
 *
 *  Created on: Nov 4, 2022
 *      Author: yasmi
 */

#include "timer1.h"


static void (*g_callBackFunction)(void);

/**********************function definitions***********************/
void Timer1_init(Timer1_ConfigType * Config_Ptr){


	TCCR1A = (1<<FOC1A) | (1<<FOC1B); /* set on any non_pwm mode */


	switch(Config_Ptr->mode)
	{
	case NORMAL:
		TCCR1A = (TCCR1A & 0xFC);  /* CLEAR WGM10 & WGM11 */
		TCCR1B = (TCCR1B & 0xE7); /* CLEAR WGM12 & WGM13 */
		TIMSK = (TIMSK & 0xC3) | (1<<TOIE1); /* enable OVERFLOW mode */
		break;


	case COMPARE:
		OCR1A = Config_Ptr->compare_value;
		TCCR1A = (TCCR1A & 0xFC);  /* CLEAR WGM10 & WGM11 */
		TCCR1B = (TCCR1B & 0xE7) | (1<<WGM12); /* CLEAR WGM13, SET WGM12 */
		TIMSK = (TIMSK & 0xC3) | (1<<OCIE1A); /* enable COMPARE OUTPUT A mode */
		break;
	}

	/* adjust the CLK presccaler */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler & 0x07)<<CS10;

	/* write initial value in the counter register */
	TCNT1 = Config_Ptr->initial_value;
}

void Timer1_deInit(void){

	/* clear timer interrupt enable bit */
	TIMSK &= ~(1<<OCIE1A) & ~(1<<TOIE1);

	/* disable the CLK presccaler */

	TCCR1B = (TCCR1B & 0xF8);

}

void Timer1_setCallBack(void(*a_ptr)(void)){

	g_callBackFunction = a_ptr;
}

void Timer1_setCompareValue(uint16 value){

	OCR1A = value;
}

ISR(TIMER1_COMPA_vect){

	(*g_callBackFunction)();
}
ISR(TIMER1_OVF_vect){

	(*g_callBackFunction)();

}

