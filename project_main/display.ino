//Function for the different pages menu
//Main Page display
void MainPage() {
  display.clearScreen();
  timedisplay();
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.setCursor(0, 45);
  display.print("< Settings");
  display.setCursor(65, 45);
  display.print("View >");
  display.setFont(liberationSansNarrow_16ptFontInfo);
}
//View page display
void ViewPage() {
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  displaydate(0);
  unsigned long startTime = millis();
  while (millis() - startTime < 200)ViewpageBtn();
}
//Settings Page display
void Settings() {
  display.setFont(thinPixel7_10ptFontInfo);
  int width = display.getPrintWidth("Example Text!");
  display.setCursor(48 - (width / 2), 10);
  display.setCursor(15, 10);
  display.fontColor(TS_8b_Green, TS_8b_Black);
  //display.print("Sensitivity!");
  display.setCursor(15, 25);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.print(userinput);
  unsigned long startTime = millis();
  while (millis() - startTime < 100)settingsbtn();
}

void timedisplay(){
  display.setFont(thinPixel7_10ptFontInfo);
  display.setCursor(0, 0);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.print("Date: ");
  display.print(rtc.getMonth());
  display.print(" : ");
  display.print(rtc.getDay());
  display.print("   ");
  }
