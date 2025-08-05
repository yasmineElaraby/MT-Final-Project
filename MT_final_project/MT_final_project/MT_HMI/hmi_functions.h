/*
 * hmi_functions.h
 *
 *  Created on: Nov 4, 2022
 *      Author: yasmi
 */

#ifndef HMI_FUNCTIONS_H_
#define HMI_FUNCTIONS_H_

#include "std_types.h"
#include "common_macros.h"

#define MAX_NUM_OF_TRIALS     3
#define NUM_OF_PASSWORD_CHARS 5
#define PASSWORD_BUFFER_SIZE  20

#define CONFIRMED        1
#define NOT_CONFIRMED    0

/* main system program windows */
#define GET_NEW_PASSWORD_OPTION      0
#define MAIN_SYSTEM_OPTIONS          1
#define OPEN_DOOR_OPTION             2
#define ACTIVATE_SYSTEM_ALARM_OPTION 3


#define ALARM_LOCK_TIME              1000
#define BUTTON_DELAY                 800

/***************function prototypes***********************/

/* getNewPassword()
 * function description :
 *  take 2 passwords from user and send it to control,
 *  return:
 *  0 --> 2 passwords are unmatched
 *  1 --> 2 passwords are matched and saved coreectly
 */
uint8 getNewPassword();


/* checkPassword()
 * function description :
 * take user password and send it to control,
 * return:
 * 0 --> the password doesn't match the saved one
 * 1 --> the password matched the saved one
 */
uint8 checkPassword();


/* displayMainOption()
 * function description:displays option window,
 * return:
 *  '+' --> open door option
 *  '-' --> change password option
 */
uint8 displayMainOptions();


/* openDoor()
 * function description:
 * send to control to open the door, and display door state on LCD
 */
void openDoor();


/* activateAlarm()
 * function description:
 * send to control emergency message and lock the system for 1 min
 */
void activateAlarm();

#endif /* HMI_FUNCTIONS_H_ */
