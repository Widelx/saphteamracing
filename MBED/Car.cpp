#include "Car.h"

PwmOut drivingWheel(PB_8);
PwmOut turnWheel(PC_7);

// A faire : brider les accélérations trop violentes (et les freinages ?) !!!!
// Variables static de vitesse et variable de vitesse désirée

void initWheels(void) {
  turnWheel.period_ms(20);
  drivingWheel.period_ms(20);
}

void turn(float angle) {
  float anglePWM;
  if (angle > ANGLE_ROUE_MAX) {
    angle = ANGLE_ROUE_MAX;
  } else if (angle < ANGLE_ROUE_MIN) {
    angle = ANGLE_ROUE_MIN;
  }
  anglePWM = -0.00045 * angle + 0.0765;
  turnWheel.write(anglePWM);
}

// Valeur de vitesse entre 0 et 100%
void speed(float speed, bool forward) {
  float speedPWM;
  // Sature les valeurs minimales
  if (speed < 0) {
    speed = 0;
  }
  if (forward) {
    // Si avance, sature selon la vitesse max avant
    if (speed > MAX_FORWARD_SPEED) {
      speed = MAX_FORWARD_SPEED;
    }
    // Applique le bon calcul de la vitesse
    speedPWM = -0.048 * speed + 0.08;
  } else {
    // Si n'avance pas (recule), sature selon la vitesse max arrière
    if (speed > MAX_BACKWARD_SPEED) {
      speed = MAX_BACKWARD_SPEED;
    }
    // Applique le bon calcul de la vitesse
    speedPWM = 0.012 * speed + 0.08;
  }
  // Applique le PWM de la vitesse en sortie
  drivingWheel.write(speedPWM);
}