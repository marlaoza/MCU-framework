#include "Managers/DisplayManager.h"
#include <Adafruit_PCD8544.h>

class Nokia5110Manager : public DisplayManager {
private:
    Adafruit_PCD8544* _lcd;

public:
    Nokia5110Manager(int sclk, int din, int dc, int cs, int rst, int bl) {
        _lcd = new Adafruit_PCD8544(sclk, din, dc, cs, rst);
        BACKLIGHT = bl;
        SCREEN_WIDTH = 84;
        SCREEN_HEIGHT = 48;
        SCREEN_STRIP = 1;
    }
    
    void setContrast(int contrast) {
        _lcd->setContrast(contrast);
    }

    bool begin() override {
        _lcd->begin();
        _lcd->setContrast(57);

        pinMode(BACKLIGHT, OUTPUT);
        digitalWrite(BACKLIGHT, HIGH);
        
        clear();
        return true;
    }

    void clear() override {
        _lcd->clearDisplay();
    }

    void flushCanvas(GFXcanvas16 *canvas, int16_t stripOffset) override {
        for (int16_t y = 0; y < SCREEN_STRIP; y++) {
            for (int16_t x = 0; x < SCREEN_WIDTH; x++) {
                uint16_t color = canvas->getPixel(x, y); 
                
                uint16_t monoColor = (color != 0) ? 1 : 0; 
                
                _lcd->drawPixel(x, stripOffset + y, monoColor);
            }
        }
        _lcd->display();
    }

    bool isPointInside(int16_t x, int16_t y) override {
        return (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT);
    }

};