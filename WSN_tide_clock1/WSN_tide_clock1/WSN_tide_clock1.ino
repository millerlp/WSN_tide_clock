/*  Simple test for ebay 128x64 oled run via I2C on a 
    Arduino Pro Mini 3.3V (328P), with ebay DS3231 real time
    clock module.
    
    The sketch will calculate the current tide height in
    San Diego (assuming clock is set correctly) and display
    the tide height and time on a ssd1306-controller OLED
    128x64 display. Time updates every second, tide updates
    as the last significant digit changes (10-20 seconds). 

*/

#include <Wire.h>
#include "SSD1306Ascii.h" // https://github.com/greiman/SSD1306Ascii
#include "SSD1306AsciiWire.h" // https://github.com/greiman/SSD1306Ascii
#include <SPI.h> // Required for RTClib to compile properly
#include <RTClib.h> // From https://github.com/millerlp/RTClib
// Real Time Clock setup
RTC_DS3231 RTC; // Uncomment when using this chip

// Tide calculation library setup.
// Change the library name here to predict for a different site.
#include "TidelibSanDiegoSanDiegoBay.h"
//#include "TidelibMontereyMontereyHarborCalifornia.h"
// Other sites available at http://github.com/millerlp/Tide_calculator
TideCalc myTideCalc; // Create TideCalc object 

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;

long oldmillis; // keep track of time
float results; // San Diego tide height
DateTime now; // define variable to hold date and time
//------------------------------------------------------------------------------
void setup() {
  Wire.begin();  
  RTC.begin(); 
  // Start up the oled display
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();  
  oled.setFont(Adafruit5x7);    
  oled.clear(); 
  oldmillis = millis();
}
//------------------------------------------------------------------------------
void loop() {
  // The main statement block will run once per second
  if ( oldmillis + 1000 < millis() ) {
      oldmillis = millis(); // update oldmillis
      now = RTC.now(); // update time
      char buf[20]; // declare a string buffer to hold the time result
      now.toString(buf, 20); // create the string, which will be put into 'buf'
      // Now extract the time by making another character pointer that
      // is advanced 10 places into buf to skip over the date. 
      char *subbuf = buf + 10;
      // Calculate current tide height
      results = myTideCalc.currentTide(now); 
      oled.home();
      oled.set2X();      
      oled.println("San Diego");
      oled.print(results, 3); // print tide ht. to 3 decimal places
      oled.println(" ft.");
      oled.set1X();
      oled.println("current tide height");
      oled.println();
      oled.set2X();
      oled.println(subbuf); // print hour:min:sec
  }
}

