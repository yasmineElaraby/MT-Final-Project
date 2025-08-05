/*
 * hmi_functions.c
 *
 *  Created on: Nov 4, 2022
 *      Author: yasmi
 */
#include "hmi_functions.h"
#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include <util/delay.h>
#include "timer1.h"


/*****************************definitions**************************************/
/* for MCUs synchronization */
#define ACK_ACCEPTED      1
#define CONTROL_ACK_RESET 0

/* the MSG code sent by HMI_ECU to CONTROL_ECU */
#define GET_NEW_PASS_MSG             0
#define CHECK_PASSWORD_MSG           1
#define ACTIVATE_SYSTEM_ALARM_MSG    2
#define OPEN_DOOR_MSG                3

/* door timings */
#define NUMBER_CLKS_PER_SECOND 1000
#define DOOR_ROTATION_TIME     5
#define DOOR_HOLD_TIME         3

extern uint8 g_passwordBuffer_1[PASSWORD_BUFFER_SIZE];
extern uint8 g_passwordBuffer_2[PASSWORD_BUFFER_SIZE];

/************************static functions prototype***************************************/
static void enterPassword(uint8 *password_ptr);
static void sendPasswordToControl(uint8 *password_ptr);
static void doorOpenTimerProcessing();

/******************global variables ****************************************************/
uint8 controlACK = 0; /* to get activity state of second MCU */
uint8 g_ticks    = 0; /* for door timer signal processing */

Timer1_ConfigType g_doorConfiguration = {F_CPU_1024,COMPARE,0,DOOR_ROTATION_TIME*NUMBER_CLKS_PER_SECOND};

/************function definitions ********************/
uint8 getNewPassword(){

	/* take from user his new password */
	LCD_clearScreen();
	LCD_displayString("Plz Enter Pass");
	LCD_moveCursor(1,0);

	enterPassword(g_passwordBuffer_1);

	/* ask from user to re-enter the new password */
	LCD_clearScreen();
	LCD_displayString("Plz reenter Pass");
	LCD_moveCursor(1,0);

	enterPassword(g_passwordBuffer_2);
	LCD_clearScreen();


	do{
		/* send the 2 entered passwords to CONTROL_ECU for confirmation */
		UART_sendByte(GET_NEW_PASS_MSG);

		/* wait for an ACK MSG from control */
		controlACK = UART_recieveByte();

	}while(controlACK != ACK_ACCEPTED); /* keep sending the MSG to control until it responds with an ack (used for synchronization ) */
	controlACK = CONTROL_ACK_RESET;

	/* send password version 1 */
	sendPasswordToControl(g_passwordBuffer_1);


	do{
		/* wait for an ACK MSG from control */
		controlACK = UART_recieveByte();
	}while(controlACK != ACK_ACCEPTED); /* keep sending the MSG to control until it responds with an ack (used for synchronization ) */
	controlACK = CONTROL_ACK_RESET;


	/* send password version 2 */
	sendPasswordToControl(g_passwordBuffer_2);

	do{
		/* wait for an ACK MSG from control */
		controlACK = UART_recieveByte();
	}while(controlACK != ACK_ACCEPTED); /* keep sending the MSG to control until it responds with an ack (used for synchronization ) */
	controlACK = CONTROL_ACK_RESET;


	uint8 matchFlag = UART_recieveByte();

	LCD_intgerToString(matchFlag);

	return matchFlag;
}

uint8 checkPassword(){

	/* take from user his new password */
	LCD_clearScreen();
	LCD_displayString("Enter Pass");
	LCD_moveCursor(1,0);

	enterPassword(g_passwordBuffer_1);

	do{
		/* send the password to CONTROL_ECU for confirmation */
		UART_sendByte(CHECK_PASSWORD_MSG);

		/* wait for an ACK MSG from control */
		controlACK = UART_recieveByte();
	}while(controlACK != ACK_ACCEPTED); /* keep sending the MSG to control until it responds with an ack (used for synchronization ) */
	controlACK = CONTROL_ACK_RESET;



	sendPasswordToControl(g_passwordBuffer_1);

	do{
		/* wait for an ACK MSG from control */
		controlACK = UART_recieveByte();
	}while(controlACK != ACK_ACCEPTED); /* keep sending the MSG to control until it responds with an ack (used for synchronization ) */
	controlACK = CONTROL_ACK_RESET;


	/* receive from controller the result of comparison */
	uint8 matchFlag = UART_recieveByte();

	return matchFlag;
}

uint8 displayMainOptions(){

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");

	uint8 option = KEYPAD_getPressedKey();
	_delay_ms(BUTTON_DELAY);

	return option;
}

void openDoor(){

	LCD_clearScreen();
	LCD_displayString("DOOR IS");
	LCD_displayStringRowColumn(1,0,"UNLOCKING !");

	Timer1_setCallBack(doorOpenTimerProcessing);
	Timer1_init(&g_doorConfiguration);

	do
	{
		UART_sendByte(OPEN_DOOR_MSG);
		controlACK = UART_recieveByte();
	}while(controlACK != ACK_ACCEPTED);
	controlACK = CONTROL_ACK_RESET;

}

void activateAlarm(){

	do
	{
		UART_sendByte(ACTIVATE_SYSTEM_ALARM_MSG);
		controlACK = UART_recieveByte();
	}while(controlACK != ACK_ACCEPTED);
	controlACK = CONTROL_ACK_RESET;

	_delay_ms(ALARM_LOCK_TIME);
}

void enterPassword(uint8 *password_ptr){

	for(int i =0; i<NUM_OF_PASSWORD_CHARS; i++)
	{
		password_ptr[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter(password_ptr[i]);
		_delay_ms(BUTTON_DELAY);
	}
}
void sendPasswordToControl(uint8 *password_ptr){

	for(int i=0; i<NUM_OF_PASSWORD_CHARS; i++)
	{
		UART_sendByte(password_ptr[i]);
	}
}

void doorOpenTimerProcessing(){


	g_ticks++;

	if(g_ticks == 1)
	{
		Timer1_setCompareValue(DOOR_HOLD_TIME*NUMBER_CLKS_PER_SECOND);
	}

	else if(g_ticks == 2)
	{
		Timer1_setCompareValue(DOOR_ROTATION_TIME*NUMBER_CLKS_PER_SECOND);
		LCD_displayStringRowColumn(1,0,"LOCKING !    ");
	}

	else if(g_ticks == 3)
	{
		Timer1_setCompareValue(DOOR_ROTATION_TIME*NUMBER_CLKS_PER_SECOND);

		g_ticks = 0;
		Timer1_deInit();
	}


}
