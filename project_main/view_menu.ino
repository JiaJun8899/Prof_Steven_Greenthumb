void displaydate(int clicked) {
  unsigned long startTime = millis();
  while (millis() - startTime < 200) {
    display.setCursor(15, 32 - (display.getFontHeight() / 2));
    display.println("Last watered");
    if (clicked == 0) {
      currentday = rtc.getDay();
      currentmonth = rtc.getMonth();
      currentyear = rtc.getYear();
    }
    difference = dateDiff(currentday, currentmonth, currentyear);
    if(difference>6){
      leds[0] = CRGB( 255, 0, 0); // Red
      FastLED.show();
      uint8_t sendWater[] = "Water Me!!";
      setble((uint8_t*)sendWater, sizeof(sendWater));
      }
    else{
      leds[0] = CRGB( 0, 0, 0); // OFF
      FastLED.show();
      uint8_t sendWatered[] = "Water? Yay!";
      setble((uint8_t*)sendWatered, sizeof(sendWatered));
      }
    display.setCursor(15, 50 - (display.getFontHeight() / 2));
    display.println(difference, DEC);
    display.print(" days ago");
  }
}
int dateDiff (int dayss, int months, int years)
{
  int daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int daysPassed = 0;

  for (int startYears = 21; startYears != years; startYears ++)
  {
    for (int n = 0; n < 12; n++)
    {
      daysPassed += daysPerMonth [n];
      if ((n == 1) && ((startYears % 4) == 0))
      {
        daysPassed += 1;
      }
    }
  }

  if (month != months)
  {
    for (int startMonth = 1; startMonth != months; startMonth ++)
    {
      daysPassed += daysPerMonth [startMonth - 1];
      if ((startMonth == 2) && ((year % 4) == 0))
      {
        daysPassed += 1;
      }
    }
  }

  daysPassed += dayss - day;
  return daysPassed;
}
void ViewpageBtn() {
  if (display.getButtons(TSButtonUpperLeft)) {
    displaydate(1);
    int x = count();
  } else {
    display.println(" ");
  }
}

int count() {
  currentday = rtc.getDay();
  currentmonth = rtc.getMonth();
  currentyear = rtc.getYear();
  /* Debugging
    SerialMonitorInterface.println("store Day:");
    SerialMonitorInterface.println(currentday,DEC);
    SerialMonitorInterface.println("store month:");
    SerialMonitorInterface.println(currentmonth,DEC);
    SerialMonitorInterface.println("store Year:");
    SerialMonitorInterface.println(currentyear,DEC);*/
  day = currentday;
  month = currentmonth;
  year = currentyear;
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
}
