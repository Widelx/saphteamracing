#ifndef _Car_h_
#define _Car_h_

#include "mbed.h"

#define ANGLE_ROUE_MIN -30
#define ANGLE_ROUE_MAX 30
#define MAX_FORWARD_SPEED 1.0
#define MAX_BACKWARD_SPEED 1.0

void initWheels(void);
void turn(float angle);
void speed(float speed, bool forward);

#endif