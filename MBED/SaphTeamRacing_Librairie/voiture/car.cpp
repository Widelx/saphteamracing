#include "car.h"

PwmOut drivingWheel(PIN_DRIVING);
PwmOut turnWheel(PIN_TURN);

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
  anglePWM = -0.0006428571429 * angle + 0.0765;
  turnWheel.write(anglePWM);
}

// Valeur de vitesse entre 0 et 100%
void speed(float speed, bool forward) {
  float speedPWM;
  static bool state = true;

  if (forward != state) {
    drivingWheel.write(0);
    state = forward;
  }
  // Sature les valeurs minimales
  if (speed < MIN_SPEED) {
    speed = MIN_SPEED;
  }
  if (speed > MAX_SPEED) {
    speed = MAX_SPEED;
  }
  if (forward) {
    // Applique le bon calcul de la vitesse
    speedPWM = -0.00022 * speed + 0.08;
  } else {
    // Si n'avance pas (recule), sature selon la vitesse max arriÃ¨re
    // Applique le bon calcul de la vitesse
    speedPWM = 0.00012 * speed + 0.08;
  }
  // Applique le PWM de la vitesse en sortie
  drivingWheel.write(speedPWM);
}