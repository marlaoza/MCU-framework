#include "Managers/DisplayManager.h"
#include "Managers/DisplayDrivers/GC9A01A.h"
#include "Managers/DisplayDrivers/Nokia5110.h"

DisplayManager* DisplayManager::_instance = nullptr;

DisplayManager::DisplayManager() {
}

DisplayManager* DisplayManager::getInstance() {
    if (_instance == nullptr) {
        #ifdef USE_NOKIA5110
            _instance = new Nokia5110Manager(0, 0, 0, 0, 0, 0);
        #elif defined(USE_GC9A01A)
            _instance = new GC9A01AManager(TFT_CS, TFT_DC, TFT_RES, TFT_BL);
        #endif
    }
    return _instance;
}

void DisplayManager::setBacklight(bool on){digitalWrite(BACKLIGHT,on);}
int16_t DisplayManager::getWidth() { return SCREEN_WIDTH; }
int16_t DisplayManager::getHeight() { return SCREEN_HEIGHT; }
int DisplayManager::getStrip(){return SCREEN_STRIP; }