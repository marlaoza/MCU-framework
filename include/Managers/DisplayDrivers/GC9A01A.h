#include "Managers/DisplayManager.h"
#include <Adafruit_GC9A01A.h>

class GC9A01AManager : public DisplayManager {
private:
    Adafruit_GC9A01A* _tft;
    const int16_t SCREEN_CENTER = 120;
    const int16_t SCREEN_RADIUS = 120;
   
public:
    GC9A01AManager(int cs, int dc, int rst, int bl) {
        _tft = new Adafruit_GC9A01A(cs, dc, rst);
        BACKLIGHT = bl;
        SCREEN_WIDTH = 240;
        SCREEN_HEIGHT = 240;
        SCREEN_STRIP = 30;
    }

    bool begin() override {
        _tft->begin();
        _tft->setRotation(0);

        pinMode(BACKLIGHT, OUTPUT);
        digitalWrite(BACKLIGHT, HIGH);

        clear();
        return true;
    }

    void clear() override {
        _tft->fillScreen(0);
    }

    void flushCanvas(GFXcanvas16 *canvas, int16_t stripOffset) override {
        _tft->drawRGBBitmap(0, stripOffset, canvas->getBuffer(), SCREEN_WIDTH, SCREEN_STRIP);
    }

    bool isPointInside(int16_t x, int16_t y) override {
        int32_t dx = x - SCREEN_CENTER;
        int32_t dy = y - SCREEN_CENTER;
        return (dx*dx + dy*dy) <= ((SCREEN_RADIUS-2) * (SCREEN_RADIUS-2));
    }
};