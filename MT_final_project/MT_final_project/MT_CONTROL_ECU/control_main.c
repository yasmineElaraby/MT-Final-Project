/*
 * control_main.c
 *
 *  Created on: Nov 2, 2022
 *      Author: yasmi
 */
#include "control_functions.h"
#include <avr/io.h>

/* 2 password buffers for future use */
uint8 g_Password_buffer_1[PASSWORD_BUFFER_SIZE];
uint8 g_Password_buffer_2[PASSWORD_BUFFER_SIZE];

int main()
{
	UART_init(9600);

	TWI_init();

	DcMotor_Init();

	Buzzer_init();

	SREG |= (1<<7); /* for timer1 interrupt */

	uint8 modeOfOperation;

	while(1)
	{
       modeOfOperation = UART_recieveByte(); /* receive from HMI type of operation message */

       switch(modeOfOperation)
		{

		case GET_NEW_PASS_MSG:
			getNewPassword();
			break;


		case CHECK_PASSWORD_MSG:
			checkPassword();
			break;


		case OPEN_DOOR_MSG:
			openDoor();
			break;


		case ACTIVATE_SYSTEM_ALARM_MSG:
			activateAlarm();
			break;

		}

	}

}
