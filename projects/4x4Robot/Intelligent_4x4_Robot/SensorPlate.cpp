#include "SensorPlate.h"
#include "Arduino.h"

Servo sensorPlate;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int servoAngle;
int lastMeasuredDistanceCms;

void setServoAngle(int angle) {
  sensorPlate.write(angle);
#ifdef DEBUG
  Serial.print (" %%%%%%%%%%%%% Setting Servo to ");
  Serial.println(angle);
  //    delay(SERVO_MOVE_DELAY);
#endif
}

//void rotationTest(){
//  for (int i = 0 ; i<=180 ; i+=10)
//  {
//    Serial.print("Angle is ");
//    Serial.println(i);
//    setServoAngle( i );
//  }
//
//  delay (2000);
//  for (int i = 180 ; i>=0 ; i-=10)
//  {
//    Serial.print("Angle is ");
//    Serial.println(i);
//    setServoAngle( i );
//  }
//}

//void setServoAngle(int angle){
//    bool isNegative = servoAngle > angle;
//    int currentAngle = servoAngle;
//    if(isNegative)
//    {
//        do{
//           sensorPlate.write(currentAngle);  // tell servo to go to servoAngle in variable 'angle'
//           delay(SERVO_MOVE_DELAY);   // waits SERVO_MOV_DELAY ms for the servo to reach the servoAngle
//           currentAngle -= 10;
//       }while(currentAngle > angle);
//    }
//   else{
//       do{
//           sensorPlate.write(currentAngle);  // tell servo to go to servoAngle in variable 'angle'
//           delay(SERVO_MOVE_DELAY);   // waits SERVO_MOV_DELAY ms for the servo to reach the servoAngle
//           currentAngle += 10;
//       }while(currentAngle < angle);
//   }
//
//   Serial.print (" Current Servo Angle ");
//   Serial.println(servoAngle);
//   Serial.print ("  ****** Setting Angle to " );
//   Serial.println(angle);
//
//   servoAngle = currentAngle;
////   Serial.print (" Updated Servo Angle ");
////   Serial.println (servoAngle);
////    delay (50);
//}

void resetServoAngle() {
  servoAngle = RESET_ANGLE;
  setServoAngle(RESET_ANGLE);
}

void initializeServo() {

  pinMode(TRIGGER_PIN, OUTPUT);   // set the trig pin to output (Send sound waves)
  pinMode(ECHO_PIN, INPUT);       // set the echo pin to input (receive sound waves)

  lastMeasuredDistanceCms = 0;    // set the lastMeasuredDistance = 0 to avoid collision and blindspot

  sensorPlate.attach(SERVO_PIN);  // attaches the servo on SERVO_PIN to the servo object
  resetServoAngle();
}

//void moveServoLeft(bool half){
////    Serial.print (" Moving to ");
////    Serial.print (half ? "half ": "");
////    Serial.println ("Left");
////    Serial.println (" Moving to Angle 90");
////    resetServoAngle();
////    Serial.println (" Moving to Angle 0");
//    setServoAngle(half ? (LEFT_FRONT_ANGLE - RESET_ANGLE/2) : LEFT_FRONT_ANGLE);
//}
//
//void moveServoRight(bool half){
////    Serial.print (" Moving to ");
////    Serial.print (half ? "half ": "");
////    Serial.println ("Right");
////    Serial.println (" Moving to Angle 90");
////    resetServoAngle();
////   Serial.println (" Moving to Angle 180");
//    setServoAngle(half ? (RIGHT_FRONT_ANGLE + RESET_ANGLE/2) : RIGHT_FRONT_ANGLE );
//}



long getDistance(int retryCount) {
  if (retryCount <= 0) {
    return (lastMeasuredDistanceCms < MIN_DISTANCE ) ?  0 : MAX_DISTANCE ;
  }

  delay(RECEIVE_DELAY);

  long distanceCm =  sonar.ping_cm(MAX_DISTANCE);
#ifdef DEBUG
  Serial.print(" >>>>>>>>>>>>>>>>> Pinging ... Distance =  ");
  Serial.println( distanceCm );
#endif
  //  if(sonar.check_timer())
  //  {
  lastMeasuredDistanceCms = distanceCm;
  return distanceCm ? distanceCm : getDistance(retryCount - 1);
  //  }
}


long getDistanceMedianCMs(int iterations) {
  long distanceUS =  sonar.ping_median(iterations);

#ifdef DEBUG
  Serial.print(" Median distance CM ");
#endif

  long distanceCm = NewPingConvert(distanceUS, US_ROUNDTRIP_CM);
  lastMeasuredDistanceCms = distanceCm;
#ifdef DEBUG
  Serial.println(distanceCm  );
#endif
  return distanceCm;
}

/**
   Get Distance using the scan
*/
long getDistance_Cms() {
#ifdef SIMULATE_SONAR
  return random(MAX_DISTANCE);
#endif

#ifdef USE_MEDIAN
  return getDistanceMedianCMs();                            // using the default iterations = 5
#else
  return getDistance(RETRY_COUNT);
#endif
}

#ifdef DEBUG
String getDirection(int angle)
{
  switch (angle) {
    case RIGHT_ANGLE :
      return " RIGHT ";
    case RIGHT_FRONT_ANGLE :
      return " FRONT-RIGHT ";
    case RESET_ANGLE :
      return " FRONT ";
    case LEFT_ANGLE :
      return " LEFT ";
    case LEFT_FRONT_ANGLE :
      return " FRONT-LEFT ";
    default:
      return "<>";
  }
}


void printDirectionAndDistance(int angle, int distanceCms) {
  Serial.print(" Direction <");
  Serial.print( getDirection(angle));
  Serial.print("> distance - ");
  Serial.print( distanceCms);
  Serial.println("cms.");
}

#endif

long getMinDistanceAt(int currentAngle, long minDistance) {
  setServoAngle(currentAngle);
  long currDistance = getDistance_Cms();

#ifdef DEBUG
  printDirectionAndDistance(currentAngle, currDistance );
#endif

  return currDistance < minDistance ? currDistance : minDistance ;
}

long getMinDistance(int initialAngle, int stepAngle, int steps) {
  long minDistance = MAX_DISTANCE;
  int currentAngle = initialAngle;
  long currDistance = 0;
  for (int i = 0; i < steps; i++) {
#ifdef SLOW_SCAN
    delay (2000);
#endif

    minDistance = getMinDistanceAt(currentAngle - stepAngle, minDistance);
    delay(RECEIVE_DELAY);

    minDistance = getMinDistanceAt(currentAngle, minDistance);
    delay(RECEIVE_DELAY);

    minDistance = getMinDistanceAt(currentAngle + stepAngle, minDistance);
  }
  return minDistance;
}

/**
   Get the Min distance in the given @enum direction
*/
long getDistanceInDirection(DIRECTION direction) {
  long distance = 0;
  switch (direction) {
    case M_LEFT :
      distance = getMinDistance(LEFT_ANGLE, SIDE_STEP, STEP_CNT);
      break;
    case M_FORWARD_LEFT :
      distance = getMinDistance(LEFT_FRONT_ANGLE, SIDE_STEP, STEP_CNT);
      break;
    case M_FORWARD :
      distance = getMinDistance(RESET_ANGLE, FWD_STEP, STEP_CNT);
      break;
    case M_FORWARD_RIGHT :
      distance = getMinDistance(RIGHT_FRONT_ANGLE, SIDE_STEP, STEP_CNT);
      break;
    case M_RIGHT :
      distance = getMinDistance(RIGHT_ANGLE , SIDE_STEP, STEP_CNT);
      break;
    default :
      distance = 0 ;
      break;
  }
  return distance;
}



///**
//   Get the Min distance Forward
//*/
//long getForawrdDistance() {
//  return getMinDistance(RESET_ANGLE - FWD_STEP * (STEP_CNT - 1), FWD_STEP, STEP_CNT);
//}
//
///**
//   Get the Min distance on Right
//*/
//long getRightDistance() {
//  return getMinDistance(RIGHT_FRONT_ANGLE - SIDE_STEP, SIDE_STEP, STEP_CNT);
//}
//
///**
//   Get the Min distance on Left
//*/
//long getLeftDistance() {
//  return getMinDistance(LEFT_FRONT_ANGLE - SIDE_STEP * (STEP_CNT - 1), SIDE_STEP, STEP_CNT);
//}
