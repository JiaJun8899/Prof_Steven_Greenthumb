/*************************************************************************
 * TSL2572 Ambient Light Sensor Wireling Tutorial:
 * This program will print the lux value read from the sensor to  the 
 * Serial Monitor
 * 
 * Hardware by: TinyCircuits
 * Code by: Ken Burns & Laveréna Wienclaw for TinyCircuits
 *
 * Initiated: 11/29/2017 
 * Updated: 04/21/2021
 ************************************************************************/

#include <Wire.h>         // For I2C communication with sensor
#include <Wireling.h>     // For interfacing with Wirelings
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

void setup() {
  SerialMonitorInterface.begin(9600);
  Wire.begin();

  // Initialize Wireling
  Wireling.begin();
  Wireling.selectPort(3);   
  display.begin();
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
  
}

void loop() {
  delay(1000);
  float avgAmbientLightLux = get_lux_level();
  writeInput(avgAmbientLightLux);
  if(avgAmbientLightLux<50.0){
    setLed(true);
  }else{
    setLed(false);
  }
}

void setLed(bool set){
  if (set){
    // cycle through all LEDs attached to LED_PIN
      //leds[0] = CRGB( 255, 0, 0); // RED
      //leds[0] = CRGB( 0, 255, 0); // GREEN
      leds[0] = CRGB( 0, 0, 255); // BLUE
      FastLED.show();
  }else{
      // cycle through all LEDs attached to LED_PIN
      leds[0] = CRGB( 0, 0, 0); // RED
      FastLED.show();
    }
}

float  get_lux_level()
{
//  Light Sensor Stuff
//  from tinycircuits.com 
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
  SerialMonitorInterface.println((AmbientLightLux + AmbientLightLux2)/2);
  return ((AmbientLightLux + AmbientLightLux2)/2);
  
}
void writeInput(float avgAmbientLightLux){
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.clearScreen();
  int width=display.getPrintWidth("Example Text!");
  display.setCursor(48-(width/2),10);
  display.print("Lux:");
  display.print(avgAmbientLightLux);
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
