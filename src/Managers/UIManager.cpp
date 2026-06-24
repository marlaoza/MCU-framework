#include "Managers/DisplayManager.h"
#include "Managers/UIManager.h"
#include "Managers/InputManager.h"
#include <vector>
#include <Utils.h>

UIManager* UIManager::_instance = nullptr;

UIManager::UIManager() {
    this->curScreen = nullptr;
    this->dirty = true;
   
    this->canvas = new GFXcanvas16(DisplayManager::getInstance()->getWidth(), DisplayManager::getInstance()->getStrip());
}
UIManager::~UIManager() {
}

UIManager* UIManager::getInstance() {
    if (_instance == nullptr) _instance = new UIManager();
    return _instance;
}

void UIManager::changeScreen(UIElement* screen){
    this->curScreen = screen;
    if(this->curScreen && this->curScreen->getChildAmount() > 0) {
        this->curScreen->setFocusedChild(this->curScreen->getChildren()[0]);
    }
    setDirtyFlag(true);
}

void UIManager::setDirtyFlag(bool dirty){
    this->dirty = dirty;
}

UIElement* UIManager::getCurScreen(){
    return this->curScreen;
}

void UIManager::handleInput(uint8_t button){
    sysLog(LOGLEVEL_UI, LOGSTATUS_INFO, "Botao apertado", button);
    if(curScreen)curScreen->handleInput(button);
}

void UIManager::update(){
    uint8_t btn;
    while(InputManager::getInstance()->popInput(&btn)){
        this->handleInput(btn);
    }
    this->curScreen->runAnimation();
    this->render();
};

void UIManager::render(){
    if(this->dirty && this->curScreen != nullptr){
        // Adafruit_GC9A01A* _tft = DisplayManager::getInstance()->getTFT();
        for(int stripOffset = 0; stripOffset < DisplayManager::getInstance()->getWidth(); stripOffset += DisplayManager::getInstance()->getStrip()){
            if(this->curScreen){ 
                UIStyle style = this->curScreen->getStyle();
                canvas->fillScreen(style.color.value_or(0));
                // if(style.sprite != nullptr && style.sprite.value()->sprite != nullptr){
                //     canvas->drawRGBBitmap(0, 0, style.sprite.value()->sprite + (stripOffset * SCREEN_WIDTH), SCREEN_WIDTH, SCREEN_STRIP);}
                this->curScreen->render(canvas, stripOffset); 
            }
            DisplayManager::getInstance()->flushCanvas(canvas, stripOffset);
        //    _tft->drawRGBBitmap(0, stripOffset, canvas->getBuffer(), DisplayManager::getInstance()->getWidth(), SCREEN_STRIP);

        }    
        this->dirty = false;
    }

}


