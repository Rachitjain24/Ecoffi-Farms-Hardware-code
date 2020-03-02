#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SSD1306Wire.h> // legacy include: `#include "SSD1306.h"`
#include <SPI.h>

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D2, D1);  //D2=SDK  D1=SCK  As per labeling on NodeMCU
void setup() {
 
  
  Serial.println("Initializing OLED Display");
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}
 
//=========================================================================


void drawFontFaceDemo() {
  // clear the display
  display.clear();
  String t = "AUTHORISED";
  //String h = "Your Current Plants:";
  //String p = "1.Lettuce";
  //String w = "2.Choulai Red";
  //String a = "AQI = ";

  
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, t);

  //display.drawString(0, 20, p);
  //display.drawString(0, 30, w);
  //display.drawString(0, 40, a);
  display.drawString(30, 40, "60");
  display.display();
  }
