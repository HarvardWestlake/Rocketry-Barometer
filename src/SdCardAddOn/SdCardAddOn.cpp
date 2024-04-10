#include "SdCardAddOn.h"

// Create a text file on the SD with this path using short 8.3 names.
#define SFN_PATH "/DIR/TEST.TXT"

// Modify CS_PIN for your chip select pin.
#define CS_PIN SS

// Set USE_SD_H to one for SD.h or zero for SdFat.
#define USE_SD_H 0

SdCardAddOn::SdCardAddOn() {
     // Initialize the SD card
    if (!SD.begin(CS_PIN)) {
        // Handle initialization failure
        Serial.println("card.init failed"); // Use Serial.print for error messages
    }


    // Assuming Serial and SD are set up correctly
    if (SD.fatType() == FAT_TYPE_EXFAT) {
        Serial.println("Type is exFAT");
    } else {
        Serial.print("Type is FAT");
        Serial.println(SD.fatType());
    }

    Serial.print("Card size: ");
    Serial.print(SD.card()->sectorCount() * 512E-9);
    Serial.println(" GB (GB = 1E9 bytes)");
}

void SdCardAddOn::error(const char* msg) {
    Serial.println(msg);
    while (1);
}

void SdCardAddOn::loop() {
    while(1) {
        // Discard any input.
        clearSerialInput();

        // F() stores strings in flash to save RAM
        Serial.println(F("Type any character to start"));
        while (!Serial.available()) {
            yield(); // To prevent blocking the CPU in a busy wait
        }

        #if HAS_UNUSED_STACK
        Serial.print(F("FreeStack: "));
        Serial.println(FreeStack());
        #endif  // HAS_UNUSED_STACK

        if (!SD.begin(CS_PIN)) {
            SD.initErrorHalt(&Serial);
        }

        if (SD.fatType() == FAT_TYPE_EXFAT) {
            Serial.println(F("Type is exFAT"));
        } else {
            Serial.print(F("Type is FAT"));
            Serial.println(SD.fatType());
        }

        Serial.print(F("Card size: "));
        Serial.print(SD.card()->sectorCount() * 512E-9);
        Serial.println(F(" GB (GB = 1E9 bytes)"));
    }
}

void SdCardAddOn::clearSerialInput() {
    uint32_t m = micros();
    do {
        if (Serial.read() >= 0) {
        m = micros();
        }
    } while (micros() - m < 10000);
}