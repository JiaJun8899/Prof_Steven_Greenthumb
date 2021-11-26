//This file contains all the function that setting page need to use
// Polling for button press on the tiny circuit
void settingsbtn() {
  display.setCursor(35, 25);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.setCursor(0, 0);
  display.setCursor(0, 10);
  display.fontColor(TS_8b_Green, TS_8b_Black);
  display.print("Incre!  LUX");
  display.setCursor(0, 45);
  display.fontColor(TS_8b_Red, TS_8b_Black);
  display.print("Decre!");
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.setCursor(65, 45);
  display.print("Back>");
  if (display.getButtons(TSButtonLowerLeft)) {
    display.setCursor(20, 55);
    userinput--;
    delay(200);
    display.setCursor(35, 25);
    display.println("");
  } else {
    display.println("               ");
  }
  display.setCursor(0, 55);
  if (display.getButtons(TSButtonUpperLeft)) {
    display.setCursor(20, 55);
    userinput++;
    delay(200);
    display.setCursor(35, 25);
    display.println("");
  } else {
    display.println("                    ");
  }
  if (display.getButtons(TSButtonUpperRight)) {
    display.fontColor(TS_8b_Blue, TS_8b_Black);
    display.setCursor(25, 55);
    display.println("Prof Steven!");
    delay(500);
  } else {
    display.println("          ");
  }
  if (display.getButtons(TSButtonLowerRight)) {

  } else {
    display.println("                    ");
  }
  if(0>userinput){
    display.fontColor(TS_8b_Red, TS_8b_Black);
    display.setCursor(22, 55);
    display.println("INVALID!");
    userinput = 0;
    }else {
    display.println("          ");
  }
}
