/******************************************************************************
  Based upon EarthListener by Pieter Sijmons @ Velleman NV
  
  Sketch to set the  VMA301 Real Time Clock to the correct date+time 
  VMA301 has DS1302 chip on board.
  Bert Lindeman @ 2019-09-23
******************************************************************************/
String SWversion = "RTC_1v0";

/***************************************/
/* ---------- DECLARATIONS ----------- */
/***************************************/
/* --- Global Libraries --- */
#include <DS1302.h>                                     // library DS1302 for VMA301 (DS1302) from https://www.velleman.eu/

/* --- Local Libraries --- */
#include "src\Adafruit-GFX-Library\Adafruit_GFX.h"      // Core graphics library by Adafruit
#include "src\MCUFRIEND_kbv\MCUFRIEND_kbv.h"            // TFT library by David Prentice
#include "src\TFTLCD-Library\Adafruit_TFTLCD.h"         // Hardware-specific library for TFT screen by Adafruit

/* --- LCD panel with touch --- */
//be aware we use the TFT LCD on a Arduino Mega!
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
// #define touchPin 38 //pin to use for reading touchscreen
// int Xpos; int Ypos; //global positions to store touchscreen location
// unsigned long touchedTime = 0;     //time when last touchscreen interaction occured;
// int touchTimeout = 300;           // timeout between touch inputs (ms)

#define TS_MINX 100
#define TS_MAXX 920
#define TS_MINY 70
#define TS_MAXY 900
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
// Though touch is not used in this sketch....
#define LCD_CS A3         // Chip Select goes to Analog 3
#define LCD_CD A2         // Command/Data goes to Analog 2
#define LCD_WR A1         // LCD Write goes to Analog 1
#define LCD_RD A0         // LCD Read goes to Analog 0
#define LCD_RESET A4      // Normally A4, but can alternately just connect to Arduino's reset pin

/* --- VMA301 DS1302 real time clock --- */ 
// URL: https://forum.arduino.cc/index.php?topic=13779.0
// You can use Arduino Mega analog-in pins as digital input and output. 
// They're numbered 54 (analog 0) through 69 (analog 15). 
const int kCePin   = 67;  // Chip Enable  67=A13 dark blue VMA301 label RST 
const int kIoPin   = 68;  // Input/Output 68=A14 purple    VMA301 label DAT
const int kSclkPin = 69;  // Serial Clock 69=A15 grey      VMA301 label CLK

/* --- define clock object --- */
DS1302 rtc(kCePin, kIoPin, kSclkPin);

/* --- Variables needed for setting the RTC clock --- */
String dayAsString(const Time::Day day) {
  switch (day) { // 1..7
    case Time::kSunday:     return "Sunday";
    case Time::kMonday:     return "Monday";
    case Time::kTuesday:    return "Tuesday";
    case Time::kWednesday:  return "Wednesday";
    case Time::kThursday:   return "Thursday";
    case Time::kFriday:     return "Friday";
    case Time::kSaturday:   return "Saturday";
  }
  return "(unknown day)";
}

const char* months[] =
{"January", "February", "March",   
 "April",   "May",      "June",
 "July",    "August",   "September", 
 "October", "November", "December"};

byte second = 0;
byte minute = 0;
byte hour = 0;
byte weekday = 0;
byte monthday = 0;
byte month = 0;
unsigned short year = 0; // 2 byte year is not good enough, and it would NOT fit in a BYTE!
bool allowToSetClock = false; // assume no Clock action wanted

/* --- Variables needed for setting the RTC clock ---  END */


// Assign human-readable names to some common 16-bit color values:
// http://www.barth-dev.de/online/rgb565-color-picker/
#define	BLACK   0x0000
#define BLUE    0x9CFF // was 0x001F Too dark to read on a black background  
#define DARKBLUE 0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0xD69A
#define GREYY   0xAD55 

//now declare tft item
MCUFRIEND_kbv tft;    //we use a DRIVER IC ILI9341

unsigned long allSeconds;
unsigned long secsRemaining;
int runDays;
int runHours;
int runMinutes;
int runSeconds;
int prevrunMinutes = 99;
int lastSecond = 99;  //we set this value so we know that we need to print the date+time
int random_y = 0; // to have to date-time on screen not always on the same line.

/***************************************/
/* ----------- LOOP CODE ------------- */
/***************************************/
void loop(void) {
  // get time since boot: will write to global vars like runSeconds etc. 
  getTimeSinceBoot();
  
  Time t = rtc.time();

  // only update date/time on screen once per second
  if(runSeconds != lastSecond) {
    printDate(); // on the TFT display
  }  
  
  lastSecond = runSeconds;
} // END of loop()
