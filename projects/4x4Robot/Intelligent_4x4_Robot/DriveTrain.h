#ifndef _DriveTrain_h_
#define _DriveTrain_h_

#include <AFMotor.h>              // import motor shield library

#define MOTOR_SPEED_FULL 250      // Define the FULL motor Speed
#define MOTOR_SPEED_HALF 100      // Define the HALF motor Speed
#define RED_PIN A3                // Pulse the RED PIN for LED
#define GREEN_PIN A4             // Pulse the GREEN PIN for LED



void initializeDriveTrain();

bool isMovingForward();

void setMotorsSpeed(int speedToSet);

void stopMoving();
void driveForward();
void driveReverse();
void driveForwardLeft();
void driveForwardRight();
void pulseRed();
void pulseAmber();
void pulseGreen();
#endif
