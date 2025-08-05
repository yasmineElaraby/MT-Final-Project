/*
 * functions.h
 *
 *  Created on: Nov 2, 2022
 *      Author: yasmi
 */

#ifndef CONTROL_FUNCTIONS_H_
#define CONTROL_FUNCTIONS_H_

#include "std_types.h"
#include "twi.h"
#include "uart.h"
#include "dc_motor.h"
#include "external_eeprom.h"

#define MAX_NUM_OF_TRIALS     3
#define NUM_OF_PASSWORD_CHARS 5
#define PASSWORD_BUFFER_SIZE  20

#define CONFIRMED        1
#define NOT_CONFIRMED    0


#define PASSWORD_BASE_ADDRESS    0x0080   /* this address is chosen randomly */

/* the MSG code sent by HMI_ECU to CONTROL_ECU */
#define GET_NEW_PASS_MSG             0
#define CHECK_PASSWORD_MSG           1
#define ACTIVATE_SYSTEM_ALARM_MSG    2
#define OPEN_DOOR_MSG                3

/**************function prototypes *******************/
void getNewPassword();

void checkPassword();

void openDoor();

void activateAlarm();


#endif /* CONTROL_FUNCTIONS_H_ */
