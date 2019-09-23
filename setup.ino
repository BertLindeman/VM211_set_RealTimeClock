/***************************************/
/* ----------- SETUP CODE ------------ */
/***************************************/

void setup(void)
{
  const int dashlinelength = 72;
  /* --- Start serial & print sketch info --- */
  Serial.begin(115200);
  for (int i=1; i<((dashlinelength - SWversion.length())/2); i++) {
    Serial.print(F("-"));
  }
  Serial.print(F(" "));
  Serial.print(SWversion);
  Serial.print(F(" "));
  for (int i=1; i<((dashlinelength - SWversion.length())/2); i++) {
    Serial.print(F("-"));
  }
  Serial.print(F("\n"));
  for (int i=1; i<dashlinelength; i++) {
     Serial.print(F("-"));
  }
  Serial.print(F("\n"));
  Serial.print(F("Arduino is running Sketch:\n"));
  Serial.println(__FILE__);
  Serial.print(F("Compiled on: "));
  Serial.print(__DATE__);
  Serial.print(F(" at "));
  Serial.println(__TIME__);
  
  for (int i=1; i<dashlinelength; i++) {
     Serial.print(F("-"));
  }
  Serial.println(F(" "));
  
  Serial.println();


  /* --- LCD screen feedback --- */
  Serial.println(F("***LCD screen feedback***"));
  Serial.println(F("TFT LCD test"));
  #ifdef USE_ADAFRUIT_SHIELD_PINOUT
    Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
  #else
    Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
  #endif
    Serial.print(F("TFT size is ")); Serial.print(tft.width()); Serial.print(F("x")); Serial.println(tft.height());
  tft.reset();

  uint16_t identifier = tft.readID();
  if (identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if (identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if (identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  } else if (identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if (identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if (identifier == 0x7783) {
    Serial.println(F("Found ST7781 LCD driver"));
  } else if (identifier == 0x8230) {
    Serial.println(F("Found UC8230 LCD driver"));
  }
  else if (identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else { 
      if (identifier == 0x0101)
      {
        identifier = 0x9341;
        Serial.println(F("Found 0x9341 LCD driver"));
      } else {
        Serial.print(F("Unknown LCD driver chip: "));
        Serial.println(identifier, HEX);
        Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
        Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
        Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
        Serial.println(F("If using the breakout board, it should NOT be #defined!"));
        Serial.println(F("Also if using the breakout, double-check that all wiring"));
        Serial.println(F("matches the tutorial."));
        identifier = 0x9341;
      }
  }

  tft.begin(identifier);
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  // pinMode(touchPin, OUTPUT);  //pin to control reading of touchscreen
  Serial.println();

  // get a random start seed.
  randomSeed(analogRead(A10));

  /* --- facilitate controling the RTC clock, START --- */
  tft.setCursor(15, 80);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(" Setup RTC.\n\n  Watch Serial-monitor");
  printDate(); // on the TFT display  
  
  while (! SetClockValues());   // run until user responds Y or N and gives valid answers 
  if (allowToSetClock)   setClock();
  /* --- facilitate controling the RTC clock, START --- */
  
  /* --- end of boot, wait nn secs, then clear screen --- */
  delay(2000);

  Serial.println(F("***End of setup, starting loop***"));
  Serial.println();
 
  tft.fillScreen(BLACK); //clear screen

  // set the date/tome on a random line now:
  random_y = (int)random(1, 200);
   
} // END of SETUP  
