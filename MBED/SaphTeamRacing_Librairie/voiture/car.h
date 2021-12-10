#ifndef _car_h_
#define _car_h_

#include "mbed.h"

#define ANGLE_ROUE_MIN -22
#define ANGLE_ROUE_MAX 22
#define MAX_FORWARD_SPEED 1.0
#define MAX_BACKWARD_SPEED 1.0

void initWheels(void);
void turn(float angle);
void speed(float speed, bool forward);

#endif