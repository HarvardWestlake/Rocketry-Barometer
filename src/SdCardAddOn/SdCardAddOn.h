#ifndef SDCARDADDON_H
#define SDCARDADDON_H

#include "SdFat.h"
class SdCardAddOn {
public:
    SdCardAddOn();
    void error(const char* msg);
    void loop();
    void clearSerialInput();
private:

    #if USE_SD_H
    #include "SD.h"
    File file;
    #else
    #include "SdFat.h"
    // Setting ENABLE_DEDICATED_SPI to zero saves over 200 more bytes.
    #if ENABLE_DEDICATED_SPI
    #warning \
        "Set ENABLE_DEDICATED_SPI zero in SdFat/src/SdFatConfig.h for minimum size"
    #endif  // ENABLE_DEDICATED_SPI
    // Insure FAT16/FAT32 only.
    SdFat32 SD;
    // FatFile does not support Stream functions, just simple read/write.
    FatFile file;
    #endif

};

#endif