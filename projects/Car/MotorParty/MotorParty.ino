// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>
#include <Servo.h> 

// DC motor on M1
AF_DCMotor motor1(1);
// DC motor on M2
AF_DCMotor motor2(2);
//// DC motor on M3
//AF_DCMotor motor3(3);
//// DC motor on M4
//AF_DCMotor motor4(4);
// DC hobby servo
Servo servo1;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor party!");
  
  // turn on servo
  servo1.attach(9);

  // turn on motor #1
  motor1.setSpeed(200);
  motor1.run(RELEASE);

  // turn on motor #2
  motor2.setSpeed(200);
  motor2.run(RELEASE);

//  // turn on motor #3
//  motor3.setSpeed(200);
//  motor3.run(RELEASE);
//
//  // turn on motor #4
//  motor4.setSpeed(200);
//  motor4.run(RELEASE);
}

int i;

// Test the DC motor, stepper and servo ALL AT ONCE!
void loop() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
//  motor3.run(FORWARD);
//  motor4.run(FORWARD);
  
  for (i=0; i<200; i++) {
    servo1.write(i);
    
    motor1.setSpeed(i);  
    motor2.setSpeed(i);  
//    motor3.setSpeed(i*10);  
//    motor4.setSpeed(i*10);  

    delay(200);
  }
 
  for (i=200; i!=0; i--) {
    servo1.write(i-10);
    motor1.setSpeed(i);  
    motor2.setSpeed(i);  
//    motor3.setSpeed(i*10);  
//    motor4.setSpeed(i*10);  
    delay(200);
 }
 
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
//  motor3.run(BACKWARD);
//  motor4.run(BACKWARD);
  
  for (i=0; i<200; i++) {
    servo1.write(i);
    
    motor1.setSpeed(i);  
    motor2.setSpeed(i);  
//    motor3.setSpeed(i*10);  
//    motor4.setSpeed(i*10);  

    delay(200);
  }
 
  for (i=200; i!=0; i--) {
    servo1.write(i-200);
    motor1.setSpeed(i);  
    motor2.setSpeed(i);  
//    motor3.setSpeed(i*10);  
//    motor4.setSpeed(i*10);  
    delay(200);
 }
}
