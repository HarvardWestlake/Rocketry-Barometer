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
#include "MatrixDisplay/MatrixDisplay.h"


double baseline;
double bmpValues[2]; //0: Temperature, 1:Pressure
Teensy_BMP180 bmp180(&Wire);
unsigned long delaytime=300;
boolean getPressure();

MatrixDisplay matrixDisplay(14, 16, 15, 1);  // Create an instance of LedDisplay


//Teensy_BMP180 bmp180(&Wire1); //If BMP180 is connected to Wire1 bus
//Teensy_BMP180 bmp180(&Wire2); //If BMP180 is connected to Wire2 bus

void setup() {

  bmp180.begin();
  if(getPressure()){
    baseline=bmpValues[1];
  }

}

void loop() {
  // Replace "1234" with the string of numbers you want to display
  //matrixDisplay.displayNumberString("1234");
  //delay(delaytime);
  //matrixDisplay.displayNumberString("Abc-123");

  // Display bpm Value bmpValues[0]
  //matrixDisplay.displayNumberString(matrixDisplay.doubleToString(bmpValues[0], 8, 2));
  
  matrixDisplay.displayNumberString(matrixDisplay.doubleToString(bmpValues[1], 8, 2));
  //delay(delaytime);

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