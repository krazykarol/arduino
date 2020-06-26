#ifndef _Constants_h_
#define _Constants_h_

#include "Arduino.h"

#define MIN_DISTANCE 30               // Define Minimum distance in a direction to move = 30CMs
#define THRESHOLD_DISTANCE 150        // Define Threshold distance in a direction to move = 150CMs

#define MOVE_DELAY 700                // Delay for the motors to keep moving in a direction before any correction is applied


#define SCAN_DIRECTIONS 5             // Number of Directions being scanned.
#define SIDE_STEP 15                  // Define the INCREMENT ANGLE angle for the Side stepping;

enum DIRECTION {
  M_LEFT = 0,                         // Direction - LEFT
  M_FORWARD_LEFT = 1,                 // Direction - FORWARD_LEFT
  M_FORWARD = 2,                      // Direction - FORWARD
  M_FORWARD_RIGHT = 3,                // Direction - FORWARD_RIGHT
  M_RIGHT = 4,                        // Direction - RIGHT
  M_REVERSE = 10,                     // Direction - REVERSE
  M_HALT = 20                         // Direction - HALT
};

//// Sensor Simulation / Dry Run
//#define DRY_RUN                   // Enable Only Dry Run. // Comment out the line to run the tyres/motors
//#define SIMULATE_SONAR                  // Random number for the Sensor/ Comment out line to use actual Sonar
//#define SLOW_SCAN                 // Slow down the scan Rate for the Servo
#define USE_MEDIAN                  // Use Median of Scans
#define DEBUG
//// #define TS_ENABLED                // Enable TimeStamp In Logs
//#define LED_OFF
#endif
