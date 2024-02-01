#ifndef MATRIXDISPLAY_H
#define MATRIXDISPLAY_H

#include <LedControl.h>

class MatrixDisplay {
public:
    MatrixDisplay(int dataPin, int clkPin, int csPin, int numDevices);
    void displayNumberString(const String& numberString);
    byte getSegmentPattern(char c);
    String doubleToString(double value, int totalLength, int decimalPlaces);

private:
    LedControl lc;
    
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

};

#endif

