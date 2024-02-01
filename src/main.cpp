

// Pin hookups for different devices
// Save pin layouts here

// Button Switch (wire order doesnt matter) 
// Wire 1 - PIN 2
// Wire 2 - GND

// BMP180 - Barometer
// SDA - 18
// SCL - 19
// GND - GND
// VIN - 3.3V

// 8-Digit Display - LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);
// POWER - 5V
// GND - GND
// DIN - 14
// CS - 15
// CLK - 16


#include <SPI.h>
#include <Teensy_BMP180.h>
#include "MatrixDisplay/MatrixDisplay.h"

const int buttonPin = 2; // Change to the pin you are using
int displayState = 0; // 0 for temp, 1 for altitude, 2 for the third state
bool lastButtonState = HIGH; // the previous reading from the input pin
bool showStateSwitch = true;


double baseline;
double bmpValues[2]; //0: Temperature, 1:Pressure
Teensy_BMP180 bmp180(&Wire);
unsigned long delaytime=300;
boolean getPressure();

MatrixDisplay matrixDisplay(14, 16, 15, 1);  // Create an instance of LedDisplay

void handleButtonPress();
void handleDisplayOutOn8CharLED();
void printToTerminal();

// ========================================
// SETUP * RUNS ONCE BEFORE MAIN LOOP
// ========================================
void setup() {

  // Setup the push button
  pinMode(buttonPin, INPUT_PULLUP); // Set the button as input with internal pull-up

  // Setup the 8-digit LED display
  bmp180.begin();

  // Start the serial communication
  if(getPressure()){
    baseline=bmpValues[1];
  }
}

// ========================================
// MAIN LOOP
// ========================================
void loop() {
  handleButtonPress();
  handleDisplayOutOn8CharLED();
  printToTerminal();
}


boolean getPressure()
{
  if (bmp180.startTemperature() != 0) {
    if (bmp180.getTemperature(bmpValues[0]) != 0) {
      if (bmp180.startPressure() != 0) {
        if (bmp180.getPressure(bmpValues[1],bmpValues[0]) != 0) {
          return(1);
        }
      }
    }
  }
  return(0);
}

// Print the temperature, pressure, and altitude to the terminal
void printToTerminal() {
  if(getPressure()) {
    double altVal = bmp180.altitude(bmpValues[1], baseline);
    Serial.print(bmpValues[0]);
    Serial.print(" C | ");
    Serial.print(bmpValues[1]);
    Serial.print(" mb | ");
    Serial.print(altVal);
    Serial.println(" cm");
  }
}

// Handle printing the correct state to the 8-char LED display
void handleDisplayOutOn8CharLED() {
  if(getPressure()) {
    double altVal = bmp180.altitude(bmpValues[1], baseline);

    // Display based on the current state
    switch (displayState) {
        case 0:
          // Temp
          if (showStateSwitch) {
            matrixDisplay.displayNumberString("Temp C");
            delay(500);
            showStateSwitch = false;
          }
            matrixDisplay.displayNumberString(matrixDisplay.doubleToString(bmpValues[0], 8, 2));
            break;
        case 1:
          // Pressure
          if (showStateSwitch) {
            matrixDisplay.displayNumberString("Pressur");
            delay(500);
            showStateSwitch = false;
          }
            matrixDisplay.displayNumberString(matrixDisplay.doubleToString(bmpValues[1], 6, 2) + "mb");
            break;
        case 2:
          // Altitude
          if (showStateSwitch) {
            matrixDisplay.displayNumberString("Altitude");
            delay(500);
            showStateSwitch = false;
          }
          // Try and show negative value TODO - NEGATIVE VALUE NOT DISPLAYING
          if (altVal > 0) {
            matrixDisplay.displayNumberString(matrixDisplay.doubleToString(altVal, 6, 2) + "cm");
          } else {
            matrixDisplay.displayNumberString("-" + matrixDisplay.doubleToString(altVal, 5, 2) + "cm");
          }
          break;
        default:
          matrixDisplay.displayNumberString("Error");
          break;
    }
  }
}

// Switches between 3 different button states
void handleButtonPress() {
  int howManyButtonStates = 3;
  int buttonState = digitalRead(buttonPin);

  // Check for button state change from HIGH to LOW
  if (buttonState != lastButtonState) {
      // If the current state is LOW, the button went from off to on:
      if (buttonState == LOW) {
        // Toggle the display state
        displayState = (displayState + 1) % howManyButtonStates;
        showStateSwitch = true; 
      }
      // Update the last button state
      lastButtonState = buttonState;
  }
}
