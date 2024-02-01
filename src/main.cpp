/*
 *  BMP180    Teensy 4.0
 *
 *  For Wire,
 *  SDA       18
 *  SCL       19
 *
 *  For Wire1,
 *  SDA       38
 *  SCL       37
 *
 *
 *  For Wire2,
 *  SDA       4
 *  SCL       3
 *
 *  For WireX,
 *  You can see interfaces on the pinouts page: https://www.pjrc.com/teensy/pinout.html*
 *
 *
 */

// LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);
// POWER - 5V
// GND - GND
// DIN - 14
// CS - 15
// CLK - 16


#include <SPI.h>
#include <Teensy_BMP180.h>
#include <LedControl.h>
double baseline;
double bmpValues[2]; //0: Temperature, 1:Pressure
Teensy_BMP180 bmp180(&Wire);
LedControl lc=LedControl(14,16,15,1);
unsigned long delaytime=300;
boolean getPressure();

//Teensy_BMP180 bmp180(&Wire1); //If BMP180 is connected to Wire1 bus
//Teensy_BMP180 bmp180(&Wire2); //If BMP180 is connected to Wire2 bus

void setup() {

  // Display Code
    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);



  bmp180.begin();
  if(getPressure()){
    baseline=bmpValues[1];
  }

}

/*
   -- A --
  |       |
  F       B
  |       |
   -- G --
  |       |
  E       C
  |       |
   -- D -- H
*/
// HABCDEFG
byte individualNumbers[10] = {
  B01111110, // 0
  B00110000, // 1
  B01101101, // 2
  B01111001, // 3
  B00110011, // 4
  B01011011, // 5
  B01011111, // 6
  B01110000, // 7
  B01111111, // 8
  B01111011  // 9
};
byte letters[26] = {
  B01110111, // A
  B00011111, // b
  B01001110, // C
  B00111101, // d
  B01001111, // E
  B01000111, // F
  B01011110, // G
  B00110111, // H
  B00000110, // I
  B00111100, // J
  B00110111, // k (same as H)
  B00001110, // L
  B01010100, // m
  B00010101, // n
  B01111110, // O
  B01100111, // P
  B01110011, // q
  B00000101, // r
  B01011011, // S
  B00001111, // t
  B00111110, // U
  B00111110, // v (same as U)
  // W is not representable
  B00110111, // X (same as H)
  B00111011, // y
  B01011011  // Z (same as S)
};// Define the 7-segment patterns for characters and some symbols

byte getSegmentPattern(char c) {
    if (c >= '0' && c <= '9') {
        return individualNumbers[c - '0']; // individualNumbers array for 0-9
    } else if (c >= 'A' && c <= 'Z') {
        return letters[c - 'A']; // letters array for A-Z
    } else if (c >= 'a' && c <= 'z') {
        return letters[toupper(c) - 'A']; // Convert lowercase to uppercase
    }
    // Add additional cases for special symbols if needed
    return 0x00; // Default pattern (blank) for unrecognized characters
}


void displayNumberString(LedControl lc, const String& numberString) {
    int numDigits = numberString.length();
    int numDevices = lc.getDeviceCount();
    int totalSegments = numDevices * 8;

    // Pad the string to 8 characters with leading spaces
    String paddedString = numberString;
    while (paddedString.length() < totalSegments) {
        paddedString = " " + paddedString;
    }
    numDigits = paddedString.length(); // Update the number of digits

    // Clear the display first
    for (int addr = 0; addr < numDevices; ++addr) {
        lc.clearDisplay(addr);
    }

    // Display each character, starting from the rightmost segment
    for (int i = 0; i < paddedString.length(); ++i) {
        char ch = paddedString.charAt(i);
        byte pattern = getSegmentPattern(ch);

        int deviceIndex = numDevices - 1 - (i / 8); // Calculate device index
        int segmentIndex = 7 - (i % 8);             // Calculate segment index within the device

        lc.setRow(deviceIndex, segmentIndex, pattern);
    }
}



void loop() {
  // Replace "1234" with the string of numbers you want to display
  displayNumberString(lc, "1234");
  delay(delaytime);
  displayNumberString(lc, "Abc-123");
  delay(delaytime);

  if(getPressure()){
    double altVal=bmp180.altitude(bmpValues[1],baseline);
    Serial.print(bmpValues[0]);
    Serial.print(" C | ");
    Serial.print(bmpValues[1]);
    Serial.print(" mb | ");
    Serial.print(altVal);
    Serial.println(" cm");
  }
  delay(100);
}

boolean getPressure()
{
  if (bmp180.startTemperature() != 0)
  {
    if (bmp180.getTemperature(bmpValues[0]) != 0)
    {
      if (bmp180.startPressure() != 0)
      {
        if (bmp180.getPressure(bmpValues[1],bmpValues[0]) != 0)
        {
          return(1);
        }
      }
    }
  }
  return(0);
}