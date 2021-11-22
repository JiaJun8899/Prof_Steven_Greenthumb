#include <Wire.h>         // For I2C communication with sensor
#include <Wireling.h>     // For interfacing with Wirelings
#include <RTCZero.h>
#include <SPI.h>
#include <TinyScreen.h>
#include <FastLED.h>

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

TinyScreen display = TinyScreen(TinyScreenDefault);

//LED Overheads
#define LED_PIN A1 // Corresponds to Wireling Port 0 (A1 = Port 1, A2 = Port 2, A3 = Port 3)
#define NUM_LEDS 1 // This is the number of RGB LEDs connected to the pin
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
int brightness = 128;

//Light Sensor Overheads
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

//RTC overheads
byte day = 21;
byte month = 11;
byte year = 21;
byte currentday = 21;
byte currentmonth = 11;
byte currentyear = 21;
RTCZero rtc;

//Global variables for our program
bool settingpage = false, viewpage = false,homepage = true;
float userinput = 0.0;
void setup(){
  //Begin interface with 9600 baud rate
  SerialMonitorInterface.begin(9600);
  Wire.begin();
  //Initialize Wirelings
  Wireling.begin();
  Wireling.selectPort(3);
  //Initialize Display
  display.begin();
  display.setFlip(true);
  display.setBrightness(10);
  //Initialize for LED
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  pinMode(LED_PIN, OUTPUT);
  /***************************************
   SETTINGS & ADJUSTMENTS for light sensor
   ***************************************/
  //TSL2572Init(GAIN_1X);
  //TSL2572Init(GAIN_8X);
  TSL2572Init(GAIN_16X);
  //TSL2572Init(GAIN_120X);
  //Initialize the RTC
  rtc.begin();
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
  //time to be set before
  rtc.setTime(23, 59, 00);
  //Display Main Page
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
  
