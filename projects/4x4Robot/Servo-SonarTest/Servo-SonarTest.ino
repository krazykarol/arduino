#include <AFMotor.h>

/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

#include "SensorPlate.h"
#include <dht.h>



#define STEP_DELAY 100
#define ROTATION_DELAY 1000

#define RED_PIN A3
#define YELLOW_PIN A4
#define DHT22_PIN A3

Servo sensorPlate;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
dht DHT;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int lastMeasuredDistanceCms = 0;


struct
{
  uint32_t total;
  uint32_t ok;
  uint32_t crc_error;
  uint32_t time_out;
  uint32_t connect;
  uint32_t ack_l;
  uint32_t ack_h;
  uint32_t unknown;
} stat = { 0, 0, 0, 0, 0, 0, 0, 0};


void setup() {
  Serial.begin(9600); // begin serial communication

  sensorPlate.attach(SERVO_PIN);  // attaches the servo on pin 10 to the servo object
  sensorPlate.write(0);              // tell servo to go to position in variable 'pos'

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
}

void loop() {
  Serial.print(" Finished t position ");
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(YELLOW_PIN, LOW);

  for (pos = 0; pos <= 360; pos += 90) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    sensorPlate.write(pos);              // tell servo to go to position in variable 'pos'
    int distanceCms = getDistance_Cms();
    printDirectionAndDistance(pos, distanceCms);

    delay(1000);                       // waits 15ms for the servo to reach the position
  }

  Serial.println(pos);
  delay (ROTATION_DELAY);

  Serial.print(" Finished t position ");
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, HIGH);
  for (pos = 360; pos >= 0; pos -= 90 ) { // goes from 180 degrees to 0 degrees
    sensorPlate.write(pos);              // tell servo to go to position in variable 'pos'
    int distanceCms = getDistance_Cms();
    printDirectionAndDistance(pos, distanceCms);

    delay(1000);                                // waits 15ms for the servo to reach the position
  }
  Serial.println(pos);
  //  digitalWrite(RED_PIN, HIGH);
  //  digitalWrite(YELLOW_PIN, HIGH);
  //
  delay (ROTATION_DELAY);

//  printHumidityAndTemp();
}

void printDirectionAndDistance(int angle, int distanceCms) {
  Serial.print(" Direction <");
  Serial.print( getDirection(angle));
  Serial.print("> distance - ");
  Serial.print( distanceCms);
  Serial.println("cms.");
}

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

  }
}

/**
    Get Distance using the scan
*/
long getDistance(int retryCount) {
  if (retryCount <= 0) {
    return (lastMeasuredDistanceCms < 30 ) ?  0 : MAX_DISTANCE ;
  }


  delay(RECEIVE_DELAY);

  long distanceCm =  sonar.ping_cm();
  Serial.println(distanceCm);
  if (sonar.check_timer())
  {
    lastMeasuredDistanceCms = distanceCm;
    return distanceCm;
  }

  Serial.print(" XXXXXX SONAR ECHO FAILED - Retry ");
  Serial.println(4 - retryCount);

  return getDistance(retryCount - 1);
}


/**
    Get Distance using the Median
*/
long getDistanceMedian(int iterations) {


  //  delay(RECEIVE_DELAY);

  long distanceCm =  sonar.ping_median(iterations);

  Serial.print(" Median distance CM ");


  //  if (sonar.check_timer())
  //  {
  distanceCm = NewPingConvert(distanceCm, US_ROUNDTRIP_CM);
  lastMeasuredDistanceCms = distanceCm;
  Serial.println(distanceCm  );
  return distanceCm;
  //  }

  //  Serial.print(" XXXXXX SONAR ECHO FAILED - Retry ");
  //  Serial.println(4 - retryCount);

  //  return getDistance(retryCount - 1);
}


/**
    Get Distance using the scan
*/
long getDistance_Cms() {

  return getDistanceMedian(5);

  //  return getDistance(3);
}

void printHumidityAndTemp()
{
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)");

  // READ DATA
  Serial.print("DHT22, \t");

  uint32_t start = micros();
  int chk = DHT.read22(DHT22_PIN);
  uint32_t stop = micros();

  stat.total++;
  switch (chk)
  {
    case DHTLIB_OK:
      stat.ok++;
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      stat.crc_error++;
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      stat.time_out++;
      Serial.print("Time out error,\t");
      break;
    case DHTLIB_ERROR_CONNECT:
      stat.connect++;
      Serial.print("Connect error,\t");
      break;
    case DHTLIB_ERROR_ACK_L:
      stat.ack_l++;
      Serial.print("Ack Low error,\t");
      break;
    case DHTLIB_ERROR_ACK_H:
      stat.ack_h++;
      Serial.print("Ack High error,\t");
      break;
    default:
      stat.unknown++;
      Serial.print("Unknown error,\t");
      break;
  }

  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.print(DHT.temperature, 1);
  Serial.print(",\t");
  Serial.print(stop - start);
  Serial.println();

  if (stat.total % 20 == 0)
  {
    Serial.println("\nTOT\tOK\tCRC\tTO\tCON\tACK_L\tACK_H\tUNK");
    Serial.print(stat.total);
    Serial.print("\t");
    Serial.print(stat.ok);
    Serial.print("\t");
    Serial.print(stat.crc_error);
    Serial.print("\t");
    Serial.print(stat.time_out);
    Serial.print("\t");
    Serial.print(stat.connect);
    Serial.print("\t");
    Serial.print(stat.ack_l);
    Serial.print("\t");
    Serial.print(stat.ack_h);
    Serial.print("\t");
    Serial.print(stat.unknown);
    Serial.println("\n");
  }
}
