
#ifndef _LCD_Display_h_
#define _LCD_Display_h_

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
#define YBEGIN2 287
#define YBEGIN3 306
#define YEND 319
#define XSTATUS 80
#define YSTATUS 145

void initializeScreen();
void printCenterSection(int distance, int color);
void printLeftSection(int distance, int color);
void printRightSection(int distance, int color);
void printMoveDirection(String dir);
#endif
