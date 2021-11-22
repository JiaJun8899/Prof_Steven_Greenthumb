#include <RTCZero.h>
#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>
TinyScreen display = TinyScreen(TinyScreenDefault);

byte day = 21;
byte month = 11;
byte year = 21;
byte currentday = 21;
byte currentmonth = 11;
byte currentyear = 21;

RTCZero rtc;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif

void setup(void) {
  SerialMonitorInterface.begin(9600);
  rtc.begin();
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
  //time to be set before
  rtc.setTime(23,59,00);
  Wire.begin();
  display.begin();
  display.setBrightness(10);
  display.setFlip(false);
}

void loop(){
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White,TS_8b_Black);
  unsigned long startTime = millis();
  while (millis() - startTime < 3000)buttonLoop();
  displaydate(0);
}

void displaydate(int clicked){
    unsigned long startTime = millis();
    while (millis() - startTime < 3000){
      display.clearScreen();
      display.setCursor(15, 32 - (display.getFontHeight() / 2));
      display.println("Last watered");
      if(clicked == 0){
        currentday = rtc.getDay();
        currentmonth = rtc.getMonth();
        currentyear = rtc.getYear();
      }
      int x = dateDiff(currentday, currentmonth, currentyear);
      display.setCursor(15, 50 - (display.getFontHeight() / 2));
      display.println(x, DEC);
      display.print(" days ago");
      delay(2000);
    }  
}

void buttonLoop() {
  if (display.getButtons(TSButtonUpperLeft)) {
    displaydate(1);
    int x = count();
  } else {
    display.println(" ");
  }
}

int count(){
  currentday = rtc.getDay();
  currentmonth = rtc.getMonth();
  currentyear = rtc.getYear();
  /*
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


int dateDiff (int dayss, int months, int years)
{
  int daysPerMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
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
