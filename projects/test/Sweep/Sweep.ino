/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

#define SERVO_MOVE_DELAY 15 // Define Delay for the Servo to move
#define RESET_ANGLE 90     // Define the Reset angle for the Servo - initial position;
#define LEFT_ANGLE 180     // Define the Reset angle for the Servo - Left position;
#define RIGHT_ANGLE 0   // Define the Reset angle for the Servo - Right position;

Servo sensorPlate;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600); // begin serial communication
  sensorPlate.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
//  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    sensorPlate.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    sensorPlate.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }

  moveServoLeft();
  delay(1000);
  moveServoRight();
  delay(1000);
}

void moveServoLeft(){
    Serial.println (" Moving to Left");  
    Serial.println (" Moving to Angle 90");  
    resetServoPosition();
    Serial.println (" Moving to Angle 0");  
    setServoAnglePosition(LEFT_ANGLE);
}

void moveServoRight(){
    Serial.println (" Moving to Right");  
    Serial.println (" Moving to Angle 90");  
    resetServoPosition();
    Serial.println (" Moving to Angle 180");    
    setServoAnglePosition(RIGHT_ANGLE);
}

void setServoAnglePosition(int angle){ 
    boolean isPositive = pos > angle;
    int currentAngle = pos;
    if(isPositive)
    {
       do{
           sensorPlate.write(currentAngle);  // tell servo to go to position in variable 'angle'
           delay(SERVO_MOVE_DELAY);   // waits SERVO_MOV_DELAY ms for the servo to reach the position
           currentAngle--;
       }while(currentAngle>angle);
    }
   else{
       do{
           sensorPlate.write(currentAngle);  // tell servo to go to position in variable 'angle'
           delay(SERVO_MOVE_DELAY);   // waits SERVO_MOV_DELAY ms for the servo to reach the position
           currentAngle++;
       }while(currentAngle<angle);
   }
   pos = currentAngle;
}

void resetServoPosition(){ 
    setServoAnglePosition(RESET_ANGLE);
}

 

