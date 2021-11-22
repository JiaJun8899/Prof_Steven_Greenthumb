#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>

//Library must be passed the board type
//TinyScreenDefault for TinyScreen shields
//TinyScreenAlternate for alternate address TinyScreen shields
//TinyScreenPlus for TinyScreen+
TinyScreen display = TinyScreen(TinyScreenDefault);

float lumi=50;

void setup() {
  
 Wire.begin();
  display.begin();
  display.setFlip(true);
  display.setBrightness(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  writeText();
  display.setFlip(true);
  delay(150);
}

void writeText(){
 
  
  display.setFont(thinPixel7_10ptFontInfo);
  int width=display.getPrintWidth("Example Text!");
  display.setCursor(48-(width/2),10);
  display.setCursor(55,10);
  //display.fontColor(TS_8b_Green,TS_8b_Black);
  //display.print("Lux!");
  display.setCursor(35,25);
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.print(lumi);
  unsigned long startTime = millis();
  while(millis() - startTime < 100)buttonLoop();
 }



void buttonLoop() {
  display.setCursor(35,25);
  display.fontColor(TS_8b_White,TS_8b_Black);

  
  display.setCursor(0, 0);
  
  display.setCursor(0,10);
  display.fontColor(TS_8b_Green,TS_8b_Black);
  display.print("Incre!  LUX");

  display.setCursor(0,40);
  display.fontColor(TS_8b_Red,TS_8b_Black);
  display.print("Decre!");

  if (display.getButtons(TSButtonLowerLeft)) {
     display.setCursor(20,55);
    lumi = lumi - 1;
    delay(200);
    display.setCursor(35,25);
    display.println("");
  } else {
    display.println("               ");
  }
  display.setCursor(0, 55);
    if (display.getButtons(TSButtonUpperLeft)){
    display.setCursor(20,55);
    lumi = lumi + 1;
    delay(200);
    display.setCursor(35,25);
    display.println("");
    
  } else {
    display.println("                    ");
  }

  if (display.getButtons(TSButtonUpperRight)) {
    display.fontColor(TS_8b_Blue,TS_8b_Black);
    display.setCursor(30,55);
    display.println("Steven!");
  } else {
    display.println("          ");
  }

  if (display.getButtons(TSButtonLowerRight)) {
   
  } else {
    display.println("                    ");
  }
}
void readInput() {
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.clearScreen();
  display.setCursor(48 - (display.getPrintWidth("Press a Button!") / 2), 32 - (display.getFontHeight() / 2));
  display.print("Press a Button!");
  unsigned long startTime = millis();
  while (millis() - startTime < 3000)buttonLoop();
  display.clearScreen();
  display.setFlip(true);
  display.setCursor(48 - (display.getPrintWidth("Press a Button!") / 2), 32 - (display.getFontHeight() / 2));
  display.print("Press a Button!");
  startTime = millis();
  while (millis() - startTime < 3000)buttonLoop();
  display.clearScreen();
  display.setFlip(false);
}
