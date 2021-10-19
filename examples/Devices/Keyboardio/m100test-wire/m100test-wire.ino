// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//



/* Example pinmap for Bluepill I2Cs (by Testato)

 I2C-1 standard pins: PB7(sda) PB6(scl)
 Use it by "Wire" without pin declaration
  Wire.begin();

 I2C-1 alternative pins: PB9(sda) PB8(scl)
 Remap the first I2C before call begin()
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();

 I2C-2: PB11(sda) PB10(scl)
 Remap the second I2C before call begin()
  Wire.setSDA(PB11);
  Wire.setSCL(PB10);
  Wire.begin();

 If you want to use the two I2Cs simultaneously, create a new instance for the second I2C
  TwoWire Wire2(PB11,PB10);
  Wire2.begin();

*/


#include <SoftWire.h>
struct cRGB {
  uint8_t b;
  uint8_t g;
  uint8_t r;
};
#define CRGB(r,g,b) (cRGB){b, g, r}

uint16_t rainbow_hue = 0;   // stores 0 to 614
cRGB rainbow;


#define ELEMENTS(arr)  (sizeof(arr) / sizeof((arr)[0]))


SoftWire SWire(PB6, PB7, SOFT_FAST);

// From http://web.mit.edu/storborg/Public/hsvtorgb.c - talk to Scott about licensing
void hsvToRgb(cRGB *cRGB, uint16_t h, uint16_t s, uint16_t v)  {
  // HSV to RGB conversion function with only integer math
  uint16_t region, fpart, p, q, t;

  if (s == 0) {
    // color is grayscale 
    cRGB->r = cRGB->g = cRGB->b = v;
    return;
  }

  // make hue 0-5 
  region = h / 43;
  // find remainder part, make it from 0-255 
  fpart = (h - (region * 43)) * 6;

  // calculate temp vars, doing integer multiplication 
  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * fpart) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

  // assign temp vars based on color cone region 
  switch (region) {
  case 0:
    cRGB->r = v;
    cRGB->g = t;
    cRGB->b = p;
    break;
  case 1:
    cRGB->r = q;
    cRGB->g = v;
    cRGB->b = p;
    break;
  case 2:
    cRGB->r = p;
    cRGB->g = v;
    cRGB->b = t;
    break;
  case 3:
    cRGB->r = p;
    cRGB->g = q;
    cRGB->b = v;
    break;
  case 4:
    cRGB->r = t;
    cRGB->g = p;
    cRGB->b = v;
    break;
  default:
    cRGB->r = v;
    cRGB->g = p;
    cRGB->b = q;
    break;
  }

  return;
}

uint8_t error, error2;

void setup()
{

pinMode(PB9, OUTPUT_OPEN_DRAIN);
digitalWrite(PB9, LOW);
    //Serial.begin(9600);
   //Serial.println("\nI2C Scanner");
//        Wire.begin();
SWire.begin();

}


void effect_rainbow_update() {
  rainbow_hue += 1;
  if (rainbow_hue >= 255)          {
    rainbow_hue %= 255;
  }
  hsvToRgb(&rainbow, rainbow_hue, 255, 100);
  uint8_t data[] = {0x03,
rainbow.b,
rainbow.g,
rainbow.r              
                   };
/*
                 if (!error) {
                 SWire.beginTransmission(0x5B);

  uint8_t result = SWire.write( data, ELEMENTS(data));
  error = SWire.endTransmission();
                 } else {

                                   SWire.beginTransmission(0x5B);
  error = SWire.endTransmission();

                 }
*/
                 if (!error2) {

SWire.beginTransmission(0x58);

   uint8_t result2 = SWire.write( data, ELEMENTS(data));
   error2 = SWire.endTransmission();
                 } else {
SWire.beginTransmission(0x58);
   error2 = SWire.endTransmission();

                 
                 }


}

void loop()
{
    byte error, address,r,g,b;
    int nDevices;

    //Serial.println("Scanning...");

    nDevices = 0;

effect_rainbow_update();
/*
    for (r=0;r<255;r+=10) {
    for (g=0;g<255;g+=10) {
    for (b=0;b<255;b+=10) {


  SWire.beginTransmission(0x58);
  uint8_t data[] = {0x03,
r,g,b
              
                   };
  uint8_t result = Wire.write( data, ELEMENTS(data));
  Wire.endTransmission();

*/

    delay(10);          // wait 2m seconds for next scan
     //   }}}

}
