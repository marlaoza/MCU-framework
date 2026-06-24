#include "Managers/DisplayManager.h"
#include <Adafruit_SPITFT.h>

class ColorManager : public DisplayManager {
private:
    Adafruit_SPITFT* _tft;
   
public:
    ColorManager(Adafruit_SPITFT* driver, int16_t w, int16_t h, int strip, int bl, bool round = false) : _tft(driver) {
        BACKLIGHT = bl;
        SCREEN_WIDTH = w;
        SCREEN_HEIGHT = h;
        SCREEN_STRIP = strip;
        isRound = round;
    }

    bool begin() override {
        _tft->begin(0);
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

};