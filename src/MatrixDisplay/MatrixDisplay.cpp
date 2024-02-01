#include "MatrixDisplay.h"

MatrixDisplay::MatrixDisplay(int dataPin, int clkPin, int csPin, int numDevices)
    : lc(dataPin, clkPin, csPin, numDevices) {
    // Initialization code here (if needed)
        
    lc.shutdown(0,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(0,8);
    /* and clear the display */
    lc.clearDisplay(0);
}

byte MatrixDisplay::getSegmentPattern(char c) {
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

void MatrixDisplay::displayNumberString(const String& numberString) {
    int numDevices = lc.getDeviceCount();
    unsigned int totalSegments = numDevices * 8 > 0 ? (unsigned int)numDevices * 8 : 1;

    // Pad the string to 8 characters with leading spaces
    String paddedString = numberString;
    while (paddedString.length() < totalSegments) {
        paddedString = " " + paddedString;
    }

    // Clear the display first
    for (int addr = 0; addr < numDevices; ++addr) {
        lc.clearDisplay(addr);
    }

    // Display each character, starting from the rightmost segment
    for (unsigned int i = 0; i < paddedString.length(); ++i) {
        char ch = paddedString.charAt(i);
        byte pattern = getSegmentPattern(ch);

        int deviceIndex = numDevices - 1 - (i / 8); // Calculate device index
        int segmentIndex = 7 - (i % 8);             // Calculate segment index within the device

        lc.setRow(deviceIndex, segmentIndex, pattern);
    }
}

String MatrixDisplay::doubleToString(double value, int totalLength, int decimalPlaces) {
    // Limit the totalLength to some reasonable size
    if (totalLength > 20) totalLength = 20;

    // Separate integer and fractional parts
    long integerPart = static_cast<long>(value);
    long fractionalPart = static_cast<long>((value - integerPart) * pow(10, decimalPlaces));

    // Convert to String
    String intStr = String(integerPart);
    String fracStr = String(abs(fractionalPart)); // abs() to handle negative values

    // Ensure fractional part has correct number of digits (leading zeros if necessary)
    while (fracStr.length() < decimalPlaces) {
        fracStr = "0" + fracStr;
    }

    // Concatenate integer and fractional parts
    String result = intStr + "." + fracStr;

    // Truncate or pad the string to the desired total length
    if (result.length() < totalLength) {
        // Pad with spaces
        while (result.length() < totalLength) {
            result = " " + result;
        }
    } else if (result.length() > totalLength) {
        // Truncate
        result = result.substring(0, totalLength);
    }

    return result;
}