
#include "DriveTrain.h"
#include "Constants.h"

/**
   Setting up the 4x4' 4 motors
*/
AF_DCMotor motor_FL(1, MOTOR12_2KHZ);
AF_DCMotor motor_FR(2, MOTOR12_2KHZ);
AF_DCMotor motor_RL(3, MOTOR12_2KHZ);
AF_DCMotor motor_RR(4, MOTOR12_2KHZ);

bool isForward;

void setMotorsSpeed(int speedToSet) {

# ifndef DRY_RUN
  motor_FL.setSpeed(speedToSet);
  motor_FR.setSpeed(speedToSet);
  motor_RL.setSpeed(speedToSet);
  motor_RR.setSpeed(speedToSet);
# endif
}

bool isMovingForward() {
  return isForward;
}

void initializeDriveTrain() {
  // Set the LED Pins as the output Pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  //set the speed of the motors, between 0-255
  setMotorsSpeed(MOTOR_SPEED_FULL);

  isForward = true;
# ifndef DRY_RUN

  motor_FL.run(FORWARD);
  motor_FR.run(FORWARD);
  motor_RL.run(FORWARD);
  motor_RR.run(FORWARD);
# endif
}

/**
   Halting!

*/
void stopMoving() {
  pulseRed();
  // there's an obstacle ahead, Halt!
# ifndef DRY_RUN
  motor_FL.run(RELEASE);
  motor_FR.run(RELEASE);
  motor_RL.run(RELEASE);
  motor_RR.run(RELEASE);
# endif
  pulseRed();
  //    delay (MOVE_DELAY);
}

/**
   Drive Forward
*/
void driveForward() {
  delay (15);
  pulseGreen();
  setMotorsSpeed(MOTOR_SPEED_FULL);

# ifndef DRY_RUN
  //if there's no obstacle ahead, Go Forward!
  motor_FL.run(FORWARD);
  motor_FR.run(FORWARD);
  motor_RL.run(FORWARD);
  motor_RR.run(FORWARD);
# endif
  isForward = true;
}

/**
   Drive Reverse
*/
void driveReverse() {
  pulseAmber();
  setMotorsSpeed(MOTOR_SPEED_HALF / 4 * 3 );

# ifndef DRY_RUN
  //Go Reverse!
  motor_FL.run(BACKWARD);
  motor_FR.run(BACKWARD);
  motor_RL.run(BACKWARD);
  motor_RR.run(BACKWARD);
# endif
  isForward = false;
  delay (1000);

}

void driveForwardLeft() {
  pulseGreen();
  setMotorsSpeed(MOTOR_SPEED_HALF);

# ifndef DRY_RUN
  //Go Left! Differential Steering
  motor_FL.run(FORWARD);
  motor_FR.run(BACKWARD);
  motor_RL.run(BACKWARD);
  motor_RR.run(FORWARD);
# endif
  isForward = true;
  //    lastMeasuredFwdDistance = 0;
  delay (MOVE_DELAY);

}

void driveForwardRight() {
  pulseGreen();
  setMotorsSpeed(MOTOR_SPEED_HALF);

# ifndef DRY_RUN
  //Go Right! Differential Steering
  motor_FL.run(BACKWARD);
  motor_FR.run(FORWARD);
  motor_RL.run(FORWARD);
  motor_RR.run(BACKWARD);
# endif
  isForward = true;
  //    lastMeasuredFwdDistance = 0;
  delay (MOVE_DELAY);
}

void pulseGreen() {
#ifndef LED_OFF
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
#endif
}

void pulseRed() {
#ifndef LED_OFF
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
#endif
}

void pulseAmber() {
#ifndef LED_OFF
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
#endif
}
