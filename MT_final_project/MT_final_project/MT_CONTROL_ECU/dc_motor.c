/*********************************
 * dc_motor.c                    *
 *  Created on: Oct 6, 2022      *
 *      Author: yasmine          *
 *********************************/


#include "dc_motor.h"
#include "pwm.h"
#include "gpio.h"

/* additional function : adjust the motor speed */
static void setSpeed(uint8 speed){
	PWM_Timer0_Start(speed);
}

void DcMotor_Init(void){

	/*set the directions of motor pins */
	GPIO_setupPinDirection(MOTOR_TERMINAL_1_PORT, MOTOR_TERMINAL_1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_TERMINAL_2_PORT, MOTOR_TERMINAL_2_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(H_BRIDGE_ENABLE_PORT, H_BRIDGE_ENABLE_PIN, PIN_OUTPUT);

	/* initially the motor is off */
	DcMotor_Rotate(OFF, 0);
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed){

	setSpeed(speed);

	switch(state)
	{
	case OFF:
		GPIO_writePin(MOTOR_TERMINAL_1_PORT, MOTOR_TERMINAL_1_PIN, LOGIC_LOW);
		GPIO_writePin(MOTOR_TERMINAL_2_PORT, MOTOR_TERMINAL_2_PIN, LOGIC_LOW);
		break;
	case CLOCKWISE:
		GPIO_writePin(MOTOR_TERMINAL_1_PORT, MOTOR_TERMINAL_1_PIN, LOGIC_HIGH);
		GPIO_writePin(MOTOR_TERMINAL_2_PORT, MOTOR_TERMINAL_2_PIN, LOGIC_LOW);
		break;
	case ANTI_CLOCKWISE:
		GPIO_writePin(MOTOR_TERMINAL_1_PORT, MOTOR_TERMINAL_1_PIN, LOGIC_LOW);
		GPIO_writePin(MOTOR_TERMINAL_2_PORT, MOTOR_TERMINAL_2_PIN, LOGIC_HIGH);
		break;
	}
}
