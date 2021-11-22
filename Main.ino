#include <Wire.h>         // For I2C communication with sensor
#include <Wireling.h>     // For interfacing with Wirelings
#include <RTCZero.h>
#include <SPI.h>
#include <TinyScreen.h>
#include <FastLED.h>

// Communication address with the sensor
#define TSL2572_I2CADDR     0x39

// Sets the gain
#define   GAIN_1X 0
#define   GAIN_8X 1
#define  GAIN_16X 2
#define GAIN_120X 3

//only use this with 1x and 8x gain settings
#define GAIN_DIVIDE_6 false

// Global variable for gain value used to Read the sensor
int gain_val = 0;
//rtc
byte day = 21;
byte month = 11;
byte year = 21;
byte currentday = 21;
byte currentmonth = 11;
byte currentyear = 21;

RTCZero rtc;

//LED
#define LED_PIN A1 // Corresponds to Wireling Port 0 (A1 = Port 1, A2 = Port 2, A3 = Port 3)

#define NUM_LEDS 1 // This is the number of RGB LEDs connected to the pin
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
int brightness = 128;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

TinyScreen display = TinyScreen(TinyScreenDefault);
bool settingpage = false;
bool viewpage = false;
bool homepage = true;
float userinput = 50.0;
void setup() {
  SerialMonitorInterface.begin(9600);
  Wire.begin();

  // Initialize Wireling
  Wireling.begin();
  Wireling.selectPort(3);
  display.begin();
  display.setFlip(true);
  //setBrightness(brightness);//sets main current level, valid levels are 0-15
  display.setBrightness(10);

  //LED
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  pinMode(LED_PIN, OUTPUT);
  //***************************************
  // SETTINGS & ADJUSTMENTS
  //***************************************
  //  TSL2572Init(GAIN_1X);
  //  TSL2572Init(GAIN_8X);
  TSL2572Init(GAIN_16X);
  //TSL2572Init(GAIN_120X);
  rtc.begin();
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
  //time to be set before
  rtc.setTime(23, 59, 00);
  MainPage();
}

void loop() {
  if (homepage) {
    writeInput(get_lux_level());
  }
  if (display.getButtons(TSButtonLowerLeft) && homepage && !settingpage) {
    display.clearScreen();
    settingpage = true;
    homepage = false;
  }
  if (settingpage) {
    Settings();
    SerialMonitorInterface.println(userinput);
  }
  if (display.getButtons(TSButtonLowerRight) && !homepage && settingpage) {
    MainPage();
    settingpage = false;
    homepage = true;
  }
  if (display.getButtons(TSButtonLowerRight) && homepage) {
    ViewPage();
    homepage = false;
    viewpage = true;
  }
  if(display.getButtons(TSButtonLowerLeft) && viewpage){
    MainPage();
    viewpage = false;
    homepage = true;
    }
  if (viewpage) {
    ViewPage();
  }

}

void setLed(bool set) {
  if (set) {
    // cycle through all LEDs attached to LED_PIN
    //leds[0] = CRGB( 255, 0, 0); // RED
    //leds[0] = CRGB( 0, 255, 0); // GREEN
    leds[0] = CRGB( 0, 0, 255); // BLUE
    FastLED.show();
  } else {
    // cycle through all LEDs attached to LED_PIN
    leds[0] = CRGB( 0, 0, 0); // RED
    FastLED.show();
  }
}

float  get_lux_level()
{
  //  Light Sensor Stuff
  //  from tinycircuits.com
  delay(1000);
  Wireling.selectPort(0);
  TSL2572Init(GAIN_16X);
  float AmbientLightLux = Tsl2572ReadAmbientLight();
  Wireling.selectPort(3);
  TSL2572Init(GAIN_16X);
  float AmbientLightLux2 = Tsl2572ReadAmbientLight();
  SerialMonitorInterface.println("Lux1: ");
  SerialMonitorInterface.println(AmbientLightLux);
  SerialMonitorInterface.println("Lux2: ");
  SerialMonitorInterface.println(AmbientLightLux2);
  SerialMonitorInterface.println("Avg Lux: ");
  SerialMonitorInterface.println((AmbientLightLux + AmbientLightLux2) / 2);
  return ((AmbientLightLux + AmbientLightLux2) / 2);
}

void writeInput(float avgAmbientLightLux) {
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  //display.clearScreen();
  int width = display.getPrintWidth("Example Text!");
  display.setCursor(48 - (width / 2), 10);
  display.print("Lux:");
  display.print(avgAmbientLightLux);
  if (avgAmbientLightLux < userinput) {
    setLed(true);
  } else {
    setLed(false);
  }
}

void MainPage() {
  display.clearScreen();
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.setCursor(0, 45);
  display.print("< Settings");
  display.setCursor(65, 45);
  display.print("View >");
  display.setFont(liberationSansNarrow_16ptFontInfo);
}

void ViewPage() {
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  unsigned long startTime = millis();
  while (millis() - startTime < 200)ViewpageBtn();
  displaydate(0);
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

void displaydate(int clicked) {
  unsigned long startTime = millis();
  while (millis() - startTime < 200) {
    display.clearScreen();
    display.setCursor(15, 32 - (display.getFontHeight() / 2));
    display.println("Last watered");
    if (clicked == 0) {
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

void Settings() {
  display.setFont(thinPixel7_10ptFontInfo);
  int width = display.getPrintWidth("Example Text!");
  display.setCursor(48 - (width / 2), 10);
  display.setCursor(15, 10);
  display.fontColor(TS_8b_Green, TS_8b_Black);
  display.print("Sensitivity!");
  display.setCursor(15, 25);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.print(userinput);
  unsigned long startTime = millis();
  while (millis() - startTime < 100)buttonLoop();
}
// Used to interface with the sensor by writing to its registers directly
void Tsl2572RegisterWrite(byte regAddr, byte regData) {
  Wire.beginTransmission(TSL2572_I2CADDR);
  Wire.write(0x80 | regAddr);
  Wire.write(regData);
  Wire.endTransmission();
}

// Initializes the light sensor to be ready for output
void TSL2572Init(uint8_t gain) {
  Tsl2572RegisterWrite( 0x0F, gain );//set gain
  Tsl2572RegisterWrite( 0x01, 0xED );//51.87 ms
  Tsl2572RegisterWrite( 0x00, 0x03 );//turn on
  if (GAIN_DIVIDE_6)
    Tsl2572RegisterWrite( 0x0D, 0x04 );//scale gain by 0.16
  if (gain == GAIN_1X)gain_val = 1;
  else if (gain == GAIN_8X)gain_val = 8;
  else if (gain == GAIN_16X)gain_val = 16;
  else if (gain == GAIN_120X)gain_val = 120;
}

// Read the lux value from the light sensor so we can print it out
float Tsl2572ReadAmbientLight() {
  uint8_t data[4];
  int c0, c1;
  float lux1, lux2, cpl;
  Wire.beginTransmission(TSL2572_I2CADDR);
  Wire.write(0xA0 | 0x14);
  Wire.endTransmission();
  Wire.requestFrom(TSL2572_I2CADDR, 4);
  for (uint8_t i = 0; i < 4; i++)
    data[i] = Wire.read();
  c0 = data[1] << 8 | data[0];
  c1 = data[3] << 8 | data[2];
  cpl = 51.87 * (float)gain_val / 60.0;
  if (GAIN_DIVIDE_6) cpl /= 6.0;
  lux1 = ((float)c0 - (1.87 * (float)c1)) / cpl;
  lux2 = ((0.63 * (float)c0) - (float)c1) / cpl;
  cpl = max(lux1, lux2);
  return max(cpl, 0.0);
}
void buttonLoop() {
  display.setCursor(15, 25);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.print(userinput);

  display.setCursor(0, 0);

  display.setCursor(0, 40);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.print("Incre!");

  display.setCursor(70, 40);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.print("Decre!");
  //getButtons() function can be used to test if any button is pressed, or used like:
  //getButtons(TSButtonUpperLeft) to test a particular button, or even like:
  //getButtons(TSButtonUpperLeft|TSButtonUpperRight) to test multiple buttons
  //results are flipped as you would expect when setFlip(true)
  display.setCursor(0, 55);
  if (display.getButtons(TSButtonLowerLeft == 1)) {
    //display.println("");
    display.setCursor(20, 55);
    userinput++;
    delay(200);
    display.setCursor(15, 25);
    display.println(userinput);
  } else {
    display.println("                    ");
  }
  if (display.getButtons(TSButtonUpperRight)) {
    display.fontColor(TS_8b_Blue, TS_8b_Black);
    display.println("Steven!");
  } else {
    display.println("       ");
  }
  if (display.getButtons(TSButtonUpperLeft)) {
    display.setCursor(20, 55);
    userinput--;
    delay(200);
    display.setCursor(15, 25);
    display.println(userinput);
  } else {
    display.println("                    ");
  }
}
