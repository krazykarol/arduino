/*
   Manik's  Intelligent 4x4 AutoDrive.

   Seeks for the Distance using the
   SONAR Sensor and displays on the LCD.

*/

#include "LCD_Display.h"          // import the Class for LCD Display
#include "SensorPlate.h"
#include "DriveTrain.h"
#include "Constants.h"
#include "DHTSensor.h"

int lastMeasuredFwdDistance;
int loopCounter;
DIRECTION currentDirection;

void setup() {

  Serial.begin(9600); // begin serial communication
  Serial.println(" *********** Intelligent 4x4 AutoDrive initialising *********** ");

  randomSeed(analogRead(0));

  currentDirection = M_HALT;

  initializeServo();

  initializeDriveTrain();

  lastMeasuredFwdDistance = 0;

  initializeScreen();

  loopCounter = 0 ;

  Serial.println(" *********** Initial setup Completed! *********** ");
}

void loop() {

  moveInDirectionOfMaxRunway();
  loopCounter++;
  if (loopCounter % 1000 == 0) {
    readData();
    displayData();
  }

}

void getDirectionalDistance(int directionalDistance[]) {
  for (int angle = 0; angle <= 180 ; angle += SIDE_STEP) {
    directionalDistance[angle / SIDE_STEP] = getMinDistanceAt(angle);
  }
}

int getDirectionalMins(int directionalMins[] , int directionalDistance[] ) {

  directionalMins [M_RIGHT] = min(directionalDistance[0], directionalDistance[1]);
  directionalMins [M_FORWARD_RIGHT] = min( min(directionalDistance[2], directionalDistance[3]) , directionalDistance[4]) ;
  directionalMins [M_FORWARD] = min( min(directionalDistance[5], directionalDistance[6]) , directionalDistance[7]) ;
  directionalMins [M_FORWARD_LEFT] = min( min(directionalDistance[8], directionalDistance[9]) , directionalDistance[10]);
  directionalMins [M_LEFT] = min(directionalDistance[11], directionalDistance[12]);

  int counter = 0;
  for (int i = 0; i < SCAN_DIRECTIONS; i++) {
    if (directionalMins[i] <= MIN_DISTANCE) {
      counter++;
    }
  }
  return counter;
}


int getProgressiveSpeed(int minCount) {
   return  (MOTOR_SPEED_HALF + MOTOR_SPEED_HALF * (1 - minCount / SCAN_DIRECTIONS));
}

void moveInDirectionOfMaxRunway() {

  /**
     Get the Sweep Scan of all the 12 positions at 15 degrees from 0 to 180

     Segemnt and get the min distance in each direction using the 30degree region at that location

     Left distance | Right distance
        - min of L|R & FL| FR

     min distance breach in one or more side
          Any side having more than min distance ? move in the direction of min distance
     all  direction breach of min distance
         - lets try reverse
                in reverse either the distance on any side changes more than threshold or
                we stop moving in reverse (forward distance remaining the same -- HALT
     All 3 (L, F, R) direction min distance > THRESHOLD
        -All clear and keep moving full speed

     Two Direction > Threshold
        - Obstacle on one side - slow down speed and keep moving till min distance in direction

     One direction > threshold
      - Obstacle on two sides -- slow down and keep moving till min distance

  */

  DIRECTION directionToTake;

  int directionalDistance[180 / SIDE_STEP + 1];
  getDirectionalDistance(directionalDistance);

  int directionalMins[SCAN_DIRECTIONS] ;
  int minCount = getDirectionalMins(directionalMins, directionalDistance);

  if (minCount == SCAN_DIRECTIONS) {
    // NO FWD PATH HALT and LIGHT up the RED
    stopMoving();
    directionToTake = M_HALT;

    // Try and Keep moving reverse while its reversing (fwd distance has to increase from last scan)
    if (lastMeasuredFwdDistance < directionalMins [M_FORWARD] ) {
      directionToTake = M_REVERSE;
    }
  } else if (minCount > 0) {
    // Move cautiously -- Slow Speed
    setMotorsSpeed(getProgressiveSpeed(minCount));
    // Find if the current movement can continue ... else turn to the direction of MAX Distance
    directionToTake = getDirectionToTake(directionalMins);

  } else {
    // min Count == 0 Open space ahead - Zoom full speed
    setMotorsSpeed(MOTOR_SPEED_FULL);

    directionToTake = M_FORWARD;
  }

  lastMeasuredFwdDistance = directionalMins [M_FORWARD] ;
  currentDirection = directionToTake;
  moveInDirection(directionToTake);
}

/**
 *   Find if the current movement can continue ... else turn to the direction of MAX Distance
 */
DIRECTION getDirectionToTake(int getDirectionalMins[] ) {
  if (getDirectionalMins[currentDirection] > THRESHOLD_DISTANCE) {
    return currentDirection;
  }

  int maxDistance = 0;
  DIRECTION directionToTake = M_HALT;     // default to halt.

  for (int i = 0; i < SCAN_DIRECTIONS; i++)
  {
    maxDistance = max(maxDistance, getDirectionalMins[i]);
    if ( maxDistance > THRESHOLD_DISTANCE) {
      directionToTake = (DIRECTION)i;
    }
  }
  return directionToTake;
}

void moveInDirectionOfMaxRunway2() {
  printTimeStamp();
  /**
     Get the fwdDistance
  */
  int  fwdDistance = getDistanceInDirection(DIRECTION::M_FORWARD);
  printDistance(fwdDistance, M_FORWARD);

  /**
     If fwdDistance > Threshold
       drive forward
      else
         Find the direction of maximum runway
         if (maxDistance > MIN_DISTANCE)
            move in direction
         else
           reverse the car

  */
  if ( fwdDistance < MIN_DISTANCE) {
    Serial.println (" Halting MIN_BARRIER Breached ");
    stopMoving();
  }

  if ( fwdDistance < THRESHOLD_DISTANCE) {
    // Slow Down
    setMotorsSpeed(MOTOR_SPEED_HALF);

#ifdef DEBUG
    printTimeStamp();
    Serial.println (" Scanning on Right... ");
#endif

    int rightDistance = getDistanceInDirection(DIRECTION::M_FORWARD_RIGHT);
    printDistance(rightDistance, M_RIGHT);


#ifdef DEBUG
    printTimeStamp();
    Serial.println (" Scanning on Left... ");
#endif
    int leftDistance = getDistanceInDirection(DIRECTION::M_FORWARD_LEFT);
    printDistance(leftDistance, M_LEFT);

    // resetServoAngle();

    int maxDistance = fwdDistance;
    DIRECTION directionToTake = M_FORWARD ;

    if (leftDistance > maxDistance) {
      maxDistance = leftDistance;
      directionToTake = M_LEFT;
    }
    if (rightDistance > maxDistance) {
      maxDistance = rightDistance;
      directionToTake = M_RIGHT;
    }

    if (maxDistance < MIN_DISTANCE) {
      // Cannot move Forward
      stopMoving();
      // Move back only if
      //     1) we were moving forward
      //     2) OR last scan of Fwd Distance is now smaller than current scan.
      if (isMovingForward() || (fwdDistance > lastMeasuredFwdDistance)) {
        //we are moving backwards
        directionToTake = M_REVERSE;
      }
      else {
        directionToTake = M_HALT;
      }
    }
    printTimeStamp();
    moveInDirection(directionToTake);
  }
  else
  {
#ifdef DEBUG
    printTimeStamp();
    Serial.println ("No obstacle detected!");
#endif
    //      resetServoAngle();

    //      setMotorsSpeed(MOTOR_SPEED_FULL);
    driveForward();
  }

  resetServoAngle();


  lastMeasuredFwdDistance = fwdDistance;
}

void moveInDirection(int direction) {
  //  Serial.print(millis());
  //  Serial.print( " - ");
#ifdef DEBUG
  Serial.print(" ******* Moving ");
#endif
  switch (direction) {
    case M_FORWARD:
#ifdef DEBUG
      Serial.print("Forward ");
#endif
      driveForward();
      printMoveDirection("******* FORWARD *******");
      break;
    case M_FORWARD_LEFT:
#ifdef DEBUG
      Serial.print("Left ");
#endif
      driveForwardLeft();
      printMoveDirection("******* FW - LEFT *******");
      break;
    case M_FORWARD_RIGHT:
#ifdef DEBUG
      Serial.print("Right ");
#endif
      driveForwardRight();
      printMoveDirection("******* FW - RIGHT *******");
      break;
    case M_REVERSE:
#ifdef DEBUG
      Serial.print("Reverse");
#endif
      driveReverse();
      printMoveDirection("******* REVERSE *******");
      break;
    default:
#ifdef DEBUG
      Serial.print("Nowhere");
#endif
      stopMoving();
      printMoveDirection("******* NOWHERE! *******");
      break;
  }
#ifdef DEBUG
  Serial.println(" ******* ");
#endif
}

word getSectionColor(int distance) {
  if (distance > THRESHOLD_DISTANCE) {
    return VGA_LIME;
  }
  else if (distance > MIN_DISTANCE) {
    return VGA_YELLOW;
  }
  else {
    return VGA_RED;
  }
}

/**
   print out the distance in centimeters.
*/
void printDistance(int distanceCMs, DIRECTION dir) {
  printDirection(dir, distanceCMs);

#ifdef DEBUG
  Serial.print (" distance From Robot is " );
  Serial.print ( distanceCMs);
  Serial.println ( " CM!");
#endif

}

void printDirection(DIRECTION dir, int lDistance) {
  switch (dir) {
    case M_FORWARD:
#ifdef DEBUG
      Serial.print("Forward");
#endif
      printCenterSection(lDistance, getSectionColor(lDistance));
      break;
    case M_LEFT:
#ifdef DEBUG
      Serial.print("Left");
#endif
      printLeftSection(lDistance, getSectionColor(lDistance));
      break;
    case M_RIGHT:
#ifdef DEBUG
      Serial.print("Right");
#endif
      printRightSection(lDistance, getSectionColor(lDistance));
      break;
    case M_REVERSE:
#ifdef DEBUG
      Serial.print("Reverse");
#endif
      break;
  }
}

void printTimeStamp() {
#ifdef TS_ENABLED
  Serial.print(millis());
  Serial.print( " - ");
#endif
}
