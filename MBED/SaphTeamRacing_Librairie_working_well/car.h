#ifndef _car_h_
#define _car_h_

#include "mbed.h"
#include "pinmap.h"

#define PIN_TURN PB_8
#define PIN_DRIVING PB_9
#define ANGLE_ROUE_MIN -22
#define ANGLE_ROUE_MAX 22
#define MAX_SPEED 100
#define MIN_SPEED 0

void initWheels(void);
void turn(float angle);
void speed(float speed, bool forward);

#endif