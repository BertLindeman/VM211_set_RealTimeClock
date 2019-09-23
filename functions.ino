/***************************************/
/* ------------ FUNCTIONS ------------ */
/***************************************/

//convert the time since boot (in milliseconds) to readable format
void getTimeSinceBoot() {
  allSeconds=     millis()      /   1000;
  runDays=        allSeconds    /   86400;
  secsRemaining=  allSeconds    %   86400;
  runHours=       secsRemaining /   3600;
  secsRemaining=  secsRemaining %   3600;
  runMinutes=     secsRemaining /   60;
  runSeconds=     secsRemaining %   60;
}


  
void printDate() {
  // Print to TFT screen  
  tft.setFont();  //standard system font
  tft.setTextSize(2);
  tft.setTextColor(GREEN,BLACK); 

  // get the date + time from the RTC
  Time t = rtc.time();


  // Show time and date on screen
  char buf[50];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  if (runMinutes != prevrunMinutes) { // change Y each minute
    
      // remove previous text
    uint16_t  x1, y1;
    uint16_t w, h;
    tft.getTextBounds(" HHHH-HH-HH HH:HH:HH ", 0, random_y, &x1, &y1, &w, &h);
    tft.fillRect(x1,y1,w,h,BLACK); // should be: BLACK);

    if (millis() > 10000) { // forget this for the first 10 seconds
        random_y = (int)random(1, 200);
    }
    prevrunMinutes = runMinutes;
    // Serial.print("runMinutes changed; using new random_y: ");
    // Serial.println(random_y); 
  }  
  tft.setCursor(15, random_y); // prevent burning in the time BUT is not as expected.... TODO
  tft.println(buf);    
}
