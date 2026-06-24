#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "HalConfig.h"
#include "Managers/DisplayManager.h"
#include "Managers/UIManager.h"
#include "Managers/InputManager.h"

class ActivityManager;

class Activity {
    protected:
        DisplayManager* _display =  DisplayManager::getInstance();
        ActivityManager* _activityManager = nullptr;
        Adafruit_GC9A01A* _tft = DisplayManager::getInstance()->getTFT();
        UIManager* _screen = UIManager::getInstance();
        InputManager* _input = InputManager::getInstance();
        std::map<std::string, UIElement*> screens;
        
    public: 
        const char* name;
        const Sprite* icon;

        Activity(const char* name, const Sprite* icon);
        virtual ~Activity() {};
        
        virtual void setup() = 0;
        virtual void loop() = 0;
        virtual void stop() = 0;
};

#endif