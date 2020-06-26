#ifndef _SERVO_h_
#define _SERVO_h_

#include <Servo.h>                    // import the Servo Library
#include <NewPing.h>                  // import the NewPing Library 
#include "Constants.h"                // import constants for simulate and dry run 

#define SERVO_PIN 10                  // define the SERVO Digital PIN
#define SERVO_MOVE_DELAY 100          // Define Delay for the Servo to move
#define RIGHT_ANGLE 0                 // Define the angle for the Servo - Right servo Angle;
#define RESET_ANGLE ( 90 - RIGHT_ANGLE )          // Define the angle for the Servo - initial servo Angle;
#define LEFT_ANGLE ( 180 - RIGHT_ANGLE )          // Define the angle for the Servo - Left servo Angle;
#define LEFT_FRONT_ANGLE 135          // Define the angle for the Servo - Left servo Angle;
#define RIGHT_FRONT_ANGLE 45          // Define the angle for the Servo - Right servo Angle;
#define STEP_CNT 1                    // Define the STEPS COUNT for the Side stepping;
#define FWD_STEP 10                   // Define the INCREMENT ANGLE angle for the Forward stepping;

#define ECHO_PIN A8                   // define the Echo pin of For UltraSonic Distance sensor;
#define TRIGGER_PIN A9                // define the Trigger pin of For UltraSonic Distance sensor;

#define SEND_DELAY 20                 // Define delay for sending the wave;
#define RECEIVE_DELAY 200             // Define delay for the sampling receive;
#define MAX_DISTANCE 300              // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm. We will use 300 cm.;

#define RETRY_COUNT 3                 // MAX Retries for the Sonar Ping 

void resetServoAngle();
void initializeServo();
long getMinDistanceAt(int currentAngle, long minDistance = 0);

long getDistanceMedianCMs(int iterations = 5);  // default iterations = 5
long getDistanceInDirection(enum DIRECTION);
#endif
