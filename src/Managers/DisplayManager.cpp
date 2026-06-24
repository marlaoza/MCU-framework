#include "Managers/DisplayManager.h"
#include "Managers/DisplayDrivers/Nokia5110.h"
#include "Managers/DisplayDrivers/ColorDriver.h"
#include <Adafruit_GC9A01A.h>

DisplayManager* DisplayManager::_instance = nullptr;

DisplayManager::DisplayManager() {
}

DisplayManager* DisplayManager::getInstance() {
    if (_instance == nullptr) {
        #ifdef USE_NOKIA5110
            _instance = new Nokia5110Manager(0, 0, 0, 0, 0, 0);
        #elif defined(USE_GC9A01A)
            Adafruit_GC9A01A* driver = new Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RES);
            _instance = new ColorManager(driver, 240, 240, 30, TFT_BL);
        #endif
    }
    return _instance;
}

bool DisplayManager::isPointInside(int16_t x, int16_t y) {
        if(isRound){
            int32_t dx = x - SCREEN_WIDTH/2;
            int32_t dy = y - SCREEN_WIDTH/2;
            return (dx*dx + dy*dy) <= ((SCREEN_WIDTH/2-2) * (SCREEN_WIDTH/2-2));
        }else{
            return (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT);
        }
    }
void DisplayManager::setBacklight(bool on){digitalWrite(BACKLIGHT,on);}
int16_t DisplayManager::getWidth() { return SCREEN_WIDTH; }
int16_t DisplayManager::getHeight() { return SCREEN_HEIGHT; }
int DisplayManager::getStrip(){return SCREEN_STRIP; }