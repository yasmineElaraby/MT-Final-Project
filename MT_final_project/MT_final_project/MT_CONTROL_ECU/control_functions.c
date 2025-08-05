/*
 * functions.c
 *
 *  Created on: Nov 2, 2022
 *      Author: yasmi
 */

#ifndef FUNCTIONS_C_
#define FUNCTIONS_C_

#include "control_functions.h"
#include <util/delay.h>

/* for motor */
#include "timer1.h"
#include "dc_motor.h"

#include "std_types.h"
#include "gpio.h"
#include "buzzer.h"

/***************************definitions*******************************/
#define NUMBER_CLKS_PER_SECOND 1000
#define DOOR_ROTATION_TIME     5
#define DOOR_HOLD_TIME         3

/* for MCUs communication */
#define ACK_ACCEPTED    1

/*********************static function prototypes ****************************/
static void saveNewPassword(uint8 *pass_ptr);
static void receivePassword(uint8 *pass_ptr);
static void motorInerruptSignalProcessing();


/**********************global variables*****************************************/

volatile uint8 g_ticks = 0; /* for timer1 interrupt processing */
volatile Timer1_ConfigType g_configuration = {F_CPU_1024,COMPARE,0,DOOR_ROTATION_TIME*NUMBER_CLKS_PER_SECOND};

extern uint8 g_Password_buffer_1[PASSWORD_BUFFER_SIZE];
extern uint8 g_Password_buffer_2[PASSWORD_BUFFER_SIZE];

/****************functions definition *************************/
void receivePassword(uint8 *password_ptr){

	for(int i =0; i<NUM_OF_PASSWORD_CHARS; i++)
	{
		password_ptr[i] = UART_recieveByte();
	}
}


void getNewPassword(){

	/* send back ACK MSG */
	UART_sendByte(ACK_ACCEPTED);

	/* receive the 2 passwords sent from HMI_ECU */
	receivePassword(g_Password_buffer_1);

	/* send back ACK MSG */
	UART_sendByte(ACK_ACCEPTED);

	receivePassword(g_Password_buffer_2);

	/* send back ACK MSG */
	UART_sendByte(ACK_ACCEPTED);

	/* compare the 2 passwords */
	for(int i=0; i<NUM_OF_PASSWORD_CHARS; i++)
	{
		if(g_Password_buffer_1[i] != g_Password_buffer_2[i])
		{
			UART_sendByte(NOT_CONFIRMED);
			return;
		}

	}

	/* once we get to this point, the 2 passwords are matched */
	UART_sendByte(CONFIRMED);
	saveNewPassword(g_Password_buffer_1);

}


void checkPassword(){

	/* send back ACK MSG */
	UART_sendByte(ACK_ACCEPTED);

	/* receive password sent from HMI_ECU */
	receivePassword(g_Password_buffer_1);

	/* send back ACK MSG */
	UART_sendByte(ACK_ACCEPTED);

	/* compare the  password with the saved one */
	uint8 readPass; /* to save password read from EEPROM */

	for(int i=0; i<NUM_OF_PASSWORD_CHARS; i++)
	{
		EEPROM_readByte(PASSWORD_BASE_ADDRESS + i , &readPass);

		if(g_Password_buffer_1[i] != readPass)
		{
			UART_sendByte(NOT_CONFIRMED);
			return;
		}

	}

	/* once we get to this point, the 2 passwords are matched */
	UART_sendByte(CONFIRMED);

}


void saveNewPassword(uint8 *pass_ptr){

	for(int i=0; i<NUM_OF_PASSWORD_CHARS; i++)
	{
		EEPROM_writeByte(PASSWORD_BASE_ADDRESS + i,pass_ptr[i]);
	}
}


void openDoor(){

	UART_sendByte(ACK_ACCEPTED);

	DcMotor_Rotate(CLOCKWISE,MOTOR_FULL_SPEED);
	Timer1_setCallBack(motorInerruptSignalProcessing);
	Timer1_init(&g_configuration);

}


void activateAlarm(){

	UART_sendByte(ACK_ACCEPTED);

	Buzzer_on();
	_delay_ms(1000);
	Buzzer_off();
}

void motorInerruptSignalProcessing(){

	g_ticks++;

	if(g_ticks == 1)
	{
		DcMotor_Rotate(OFF,0);
		Timer1_setCompareValue(DOOR_HOLD_TIME*NUMBER_CLKS_PER_SECOND);
	}

	else if(g_ticks == 2)
	{
		DcMotor_Rotate(ANTI_CLOCKWISE,MOTOR_FULL_SPEED);
		Timer1_setCompareValue(DOOR_ROTATION_TIME*NUMBER_CLKS_PER_SECOND);
	}

	else if(g_ticks == 3)
	{
		DcMotor_Rotate(OFF,0);
		Timer1_setCompareValue(DOOR_ROTATION_TIME*NUMBER_CLKS_PER_SECOND);

		g_ticks = 0;
		Timer1_deInit();
	}

}

#endif /* FUNCTIONS_C_ */
