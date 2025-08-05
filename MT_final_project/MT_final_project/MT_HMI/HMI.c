/*
 * HMI.c
 *
 *  Created on: Nov 4, 2022
 *      Author: yasmi
 */
#include "lcd.h"
#include "keypad.h"
#include "hmi_functions.h"
#include "uart.h"
#include <util/delay.h>
#include <avr/io.h>

/* 2 password buffers for future use */
uint8 g_passwordBuffer_1[PASSWORD_BUFFER_SIZE];
uint8 g_passwordBuffer_2[PASSWORD_BUFFER_SIZE];

uint8 g_numberOfTrials = 0; /* counter for the number of time the user enters wrong password */

uint8 firstPasswordFlag = 1; /* if first entered new password, the user is not asked to enter a checking password */

int main(){

	UART_init(9600);
	_delay_ms(100); /* giving time for processing */

	LCD_init();
	_delay_ms(100); /* giving time for processing */

	SREG |= (1<<7);  /* for timer1 interrupt */


	uint8 modeOfOperation = GET_NEW_PASSWORD_OPTION;

	while(1){

		switch(modeOfOperation)
		{

		case GET_NEW_PASSWORD_OPTION:

			if(firstPasswordFlag)
			{
				uint8 operation = getNewPassword();
				if(operation == CONFIRMED)
				{
					firstPasswordFlag = 0;
					modeOfOperation = MAIN_SYSTEM_OPTIONS;
				}
			}

			else
			{
				if (checkPassword() == CONFIRMED)
				{
					g_numberOfTrials = 0;  /* number of trials are counting only consecutive failed enters, restore it to 0 when user enters right password */

					uint8 operation = getNewPassword();
					if(operation == CONFIRMED)
					{
						modeOfOperation = MAIN_SYSTEM_OPTIONS;
					}

				}

				else
				{
					g_numberOfTrials++;
					if(g_numberOfTrials == MAX_NUM_OF_TRIALS) /* activate system alarm */
					{
						g_numberOfTrials = 0;
						modeOfOperation = ACTIVATE_SYSTEM_ALARM_OPTION;
					}
				}

			}
			break;

		case MAIN_SYSTEM_OPTIONS:

			switch(displayMainOptions())
			{

			case '+':
				if (checkPassword() == CONFIRMED)
				{
					g_numberOfTrials = 0;  /* number of trials are counting only consecutive failed enters, restore it to 0 when user enters right password */

					modeOfOperation = OPEN_DOOR_OPTION;

				}
				else
				{
					g_numberOfTrials++;

					if(g_numberOfTrials == MAX_NUM_OF_TRIALS)
					{
						g_numberOfTrials = 0;
						modeOfOperation = ACTIVATE_SYSTEM_ALARM_OPTION;
					}
				}
				break;

			case '-':
				if (checkPassword() == CONFIRMED)
				{
					g_numberOfTrials = 0;  /* number of trials are counting only consecutive failed enters, restore it to 0 when user enters right password */

					modeOfOperation = GET_NEW_PASSWORD_OPTION;
				}
				else
				{
					g_numberOfTrials++;

					if(g_numberOfTrials == MAX_NUM_OF_TRIALS)
					{
						g_numberOfTrials = 0;
						modeOfOperation = ACTIVATE_SYSTEM_ALARM_OPTION;
					}
				}
				break;

			default :
				modeOfOperation = MAIN_SYSTEM_OPTIONS;
			}

			break;


		case OPEN_DOOR_OPTION:
			openDoor();
			break;


		case ACTIVATE_SYSTEM_ALARM_OPTION:
			activateAlarm();
			break;

		}




	}
}

