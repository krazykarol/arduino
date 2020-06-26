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
#include "LCD_Display.h"


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


//void setup()
//{
//  randomSeed(analogRead(0));
//
//  // Setup the LCD
//  myGLCD.InitLCD();
//  myGLCD.setFont(BigFont);
//}

//void loop()
//{
//
//  initializeScreen();
//
//  delay(500);
//
//  int lDistance = random(400);
//  clearLeftSection(lDistance);
//  printLeftSection(lDistance);
//
//  delay(500);
//
//  lDistance = random(400);
//  clearCenterSection(lDistance);
//  printCenterSection(lDistance);
//
//  delay(500);
//
//  lDistance = random(400);
//  clearRightSection(lDistance);
//  printRightSection(lDistance);
//
//
//  delay(2000);
//

//}

void initializeScreen() {
  int buf[478];
  int x, x2;
  int y, y2;
  int r;

  // Setup the LCD
  myGLCD.InitLCD();           // LANDSCAPE (default)
  myGLCD.setFont(BigFont);

  // Clear the screen and draw the frame
  myGLCD.clrScr();

  //  myGLCD.setRotation(1)

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(XBEGIN, YBEGIN, XEND, YBEGIN1 - 1);
  myGLCD.setColor(64, 255, 64);
  myGLCD.fillRect(XBEGIN, YBEGIN2, XEND, YBEGIN3 - 1);

  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(XBEGIN, YBEGIN3, XEND, YEND);

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


void clearScreenSection(int x, int y, int x2, int y2, word color) {
  myGLCD.setColor(color);
  myGLCD.fillRect(x, y, x2, y2);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(color);
}

void clearLeftSection(word color) {
  clearScreenSection(XBEGIN, YBEGIN1, XBEGIN1 - 1, YBEGIN2 - 1, color);
}
void clearCenterSection(word color) {
  clearScreenSection(XBEGIN1, YBEGIN1, XBEGIN2 - 1, YBEGIN2 - 1, color);
}
void clearRightSection(word color) {
  clearScreenSection(XBEGIN2, YBEGIN1, XEND, YBEGIN2 - 1, color);
}

void printSpeedInSection(String str, int x, int y) {
  myGLCD.print(str, x, y);
}

void printLeftSection(int distance, int color) {
  clearLeftSection(color);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("LEFT"), XBEGIN + 45, (YBEGIN2 - YBEGIN1) / 2 - 60);
  myGLCD.setFont(SevenSegNumFont);
  printSpeedInSection(String(distance), XBEGIN + 25, (YBEGIN2 - YBEGIN1) / 2 - 20);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("CMs"), XBEGIN + 55, (YBEGIN2 - YBEGIN1) / 2 + 60);
}

void printCenterSection(int distance, int color) {
  clearCenterSection(color);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("CENTER"), XBEGIN1 + 30, (YBEGIN2 - YBEGIN1) / 2 - 60);
  myGLCD.setFont(SevenSegNumFont);
  printSpeedInSection(String(distance), XBEGIN1 + 25, (YBEGIN2 - YBEGIN1) / 2 - 20);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("CMs"), XBEGIN1 + 55, (YBEGIN2 - YBEGIN1) / 2 + 60);
}

void printRightSection(int distance, int color) {
  clearRightSection(color);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("RIGHT"), XBEGIN2 + 40, (YBEGIN2 - YBEGIN1) / 2 - 60);
  myGLCD.setFont(SevenSegNumFont);
  printSpeedInSection(String(distance), XBEGIN2 + 25, (YBEGIN2 - YBEGIN1) / 2 - 20);
  myGLCD.setFont(BigFont);
  printSpeedInSection(String("CMs"), XBEGIN2 + 55, (YBEGIN2 - YBEGIN1) / 2 + 60);
}

void printMoveDirection(String dir) {
  myGLCD.setFont(BigFont);
  printSpeedInSection(String(dir), CENTER, (YBEGIN2 + 5));
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
