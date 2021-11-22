//This file controls the LED lighting up and taking in the values from the light sensor and averaging it

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
  for (uint8_t i = 0; i < 4; i++){
    data[i] = Wire.read();
  }
  c0 = data[1] << 8 | data[0];
  c1 = data[3] << 8 | data[2];
  cpl = 51.87 * (float)gain_val / 60.0;
  if (GAIN_DIVIDE_6) cpl /= 6.0;
  lux1 = ((float)c0 - (1.87 * (float)c1)) / cpl;
  lux2 = ((0.63 * (float)c0) - (float)c1) / cpl;
  cpl = max(lux1, lux2);
  return max(cpl, 0.0);
}
//Function to get and average out the Lux Level from sensors
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
  //Debugger
  /*SerialMonitorInterface.println("Lux1: ");
  SerialMonitorInterface.println(AmbientLightLux);
  SerialMonitorInterface.println("Lux2: ");
  SerialMonitorInterface.println(AmbientLightLux2);
  SerialMonitorInterface.println("Avg Lux: ");
  SerialMonitorInterface.println((AmbientLightLux + AmbientLightLux2) / 2);*/
  return ((AmbientLightLux + AmbientLightLux2) / 2);
}
//Function to control the LED
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
//Displays the LUX level onto the screen
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
