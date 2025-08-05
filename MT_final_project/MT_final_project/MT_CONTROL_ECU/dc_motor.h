/*********************************
 * dc_motor.h                    *
 *  Created on: Oct 6, 2022      *
 *      Author: yasmine          *
 *********************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

typedef enum{
	OFF, CLOCKWISE, ANTI_CLOCKWISE
}DcMotor_State;

/*****definitions*****/
#define MOTOR_TERMINAL_1_PORT  PORTA_ID
#define MOTOR_TERMINAL_1_PIN   PIN0_ID

#define MOTOR_TERMINAL_2_PORT  PORTA_ID
#define MOTOR_TERMINAL_2_PIN   PIN1_ID

#define H_BRIDGE_ENABLE_PORT   PORTA_ID
#define H_BRIDGE_ENABLE_PIN    PIN2_ID

/* ASSUMPTION: default rotation direction is clockwise */
#define DEFAULT_ROTATION_DIRECTION  CLOCKWISE
#define MOTOR_FULL_SPEED            100

/******functions prototypes*******/
void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* DC_MOTOR_H_ */
