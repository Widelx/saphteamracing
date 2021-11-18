#include "LPC17xx.h"

void initPWM(void);
void initPWMServo(void);
void initPWMMoteurCC(void);
bool setPWMServo(unsigned float rapport_cyclique);
bool setPWMMoteur(unsigned float rapport_cyclique);