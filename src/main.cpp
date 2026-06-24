#include <Arduino.h>
#include "HalConfig.h"
#include "Managers/DisplayManager.h"
#include "Managers/ActivityManager.h"
#include "Managers/InputManager.h"
#include "Managers/StorageManager.h"
#include "Activities/Menu.h"

DisplayManager* display;
UIManager* screen;
ActivityManager* activity;
InputManager* input;
StorageManager* storage;

Menu* menuActivity = nullptr;

void setup() {
    Serial.begin(2000000);

    display = DisplayManager::getInstance();
    display->begin();
    input = InputManager::getInstance();
    input->begin();
    storage = StorageManager::getInstance();
    storage->begin(true);
    activity = ActivityManager::getInstance();
    screen = UIManager::getInstance();

    menuActivity = new Menu();
    activity->registerActivity(menuActivity);
    activity->setActivity("menu");
}

void loop() {
    activity->loopActivity();
}