#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "UI/UIElement.h"

class UIManager {
private:
    static UIManager* _instance;
    UIManager();
    ~UIManager();
    
    UIElement* curScreen;
    uint16_t* _psramBuffer = nullptr;
    GFXcanvas16* canvas;

    bool dirty;
    void render();
public:
    void handleInput(uint8_t button);
    static UIManager* getInstance();
    void changeScreen(UIElement* screen);
    UIElement* getCurScreen();
    void setDirtyFlag(bool dirty);

    void update();
};

#endif