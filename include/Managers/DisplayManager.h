#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_GFX.h>
#include "HalConfig.h"

class DisplayManager {
protected:
    static DisplayManager* _instance;
    DisplayManager();
    
    int16_t SCREEN_WIDTH;
    int16_t SCREEN_HEIGHT;
    int SCREEN_STRIP;
    int BACKLIGHT;

public:
    static DisplayManager* getInstance();

    virtual bool begin() = 0;
    virtual void clear() = 0;
    virtual void flushCanvas(GFXcanvas16 *canvas, int16_t stripOffset) = 0;
    virtual bool isPointInside(int16_t x, int16_t y) = 0;

    void setBacklight(bool on);
    int16_t getWidth();
    int16_t getHeight();
    int getStrip();
    
};

#endif