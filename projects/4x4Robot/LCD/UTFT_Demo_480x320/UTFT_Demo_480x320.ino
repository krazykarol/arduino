// UTFT_Demo_480x320
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution
// of 480x320 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

#define XBEGIN 0
#define XBEGIN1 160
#define XBEGIN2 320
#define XEND 479
#define YBEGIN 0
#define YBEGIN1 14
#define YBEGIN2 306
#define YEND 319
#define XSTATUS 80
#define YSTATUS 145

#define THRESHOLD_DISTANCE 200     // Define Minimum distance in a direction to move = 30CMs
#define MIN_DISTANCE 60     // Define Minimum distance in a direction to move = 30CMs


// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Arduino Uno / 2009:
// -------------------
// Standard Arduino Uno/2009 shield            : <display model>,A5,A4,A3,A2
// DisplayModule Arduino Uno TFT shield        : <display model>,A5,A4,A3,A2
//
// Arduino Mega:
// -------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Mega      : <display model>,38,39,40,41
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(CTE32HR, 38, 39, 40, 41);


void setup()
{
  randomSeed(analogRead(0));

  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
}

void loop()
{

  initializeScreen();

  delay(500);

  int lDistance = random(400);
  clearLeftSection(lDistance);
  printLeftSection(lDistance);

  delay(500);

  lDistance = random(400);
  clearCenterSection(lDistance);
  printCenterSection(lDistance);

  delay(500);

  lDistance = random(400);
  clearRightSection(lDistance);
  printRightSection(lDistance);


  delay(2000);


}

void initializeScreen() {
  int buf[478];
  int x, x2;
  int y, y2;
  int r;

  // Clear the screen and draw the frame
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 479, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 306, 479, 319);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("* Karol's 4x4 Monitor *", CENTER, 1);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255, 255, 0);
  myGLCD.print("<http://www.manikkarol.com/>", CENTER, 307);

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 479, 305);
}

//void drawCurves() {
//  drawCrossHair();
//  drawWaves();
//}

void printLeftSection(int distance) {
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("LEFT"), XBEGIN + 45, (YBEGIN2-YBEGIN1)/2 - 60);
  myGLCD.setFont(SevenSegNumFont);
  printSpeedInSection(String(distance), XBEGIN + 25, (YBEGIN2-YBEGIN1)/2 - 20);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("CMs"), XBEGIN + 55, (YBEGIN2-YBEGIN1)/2 + 60);
}

void printCenterSection(int distance) {
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("CENTER"), XBEGIN1 + 30, (YBEGIN2-YBEGIN1)/2 - 60);
  myGLCD.setFont(SevenSegNumFont);
  printSpeedInSection(String(distance), XBEGIN1 + 25, (YBEGIN2-YBEGIN1)/2 - 20);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("CMs"), XBEGIN1 + 55, (YBEGIN2-YBEGIN1)/2 + 60);
}

void printRightSection(int distance) {
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("RIGHT"), XBEGIN2 + 40, (YBEGIN2-YBEGIN1)/2 - 60);
  myGLCD.setFont(SevenSegNumFont);
  printSpeedInSection(String(distance), XBEGIN2 + 25, (YBEGIN2-YBEGIN1)/2 - 20);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("CMs"), XBEGIN2 + 55, (YBEGIN2-YBEGIN1)/2 + 60);
}

void printSpeedInSection(String str, int x, int y) {
  myGLCD.print(str, x, y);
}

void clearLeftSection(int distance) {
  clearScreenSection(XBEGIN, YBEGIN1, XBEGIN1 - 1, YBEGIN2 - 1, getSectionColor(distance));
}
void clearCenterSection(int distance) {
  clearScreenSection(XBEGIN1, YBEGIN1, XBEGIN2 - 1, YBEGIN2 - 1, getSectionColor(distance));
}
void clearRightSection(int distance) {
  clearScreenSection(XBEGIN2, YBEGIN1, XEND, YBEGIN2 - 1, getSectionColor(distance));
}

void clearScreenSection(int x, int y, int x2, int y2, word color) {
  myGLCD.setColor(color);
  myGLCD.fillRect(x, y, x2, y2);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(color);
}

void drawCrossHair() {
  // Draw crosshairs
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(239, 15, 239, 304);
  myGLCD.drawLine(1, 159, 478, 159);
  for (int i = 9; i < 470; i += 10)
    myGLCD.drawLine(i, 157, i, 161);
  for (int i = 19; i < 220; i += 10)
    myGLCD.drawLine(237, i, 241, i);
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


