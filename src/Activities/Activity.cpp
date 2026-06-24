#include "Activity.h"
#include "Managers/ActivityManager.h"

Activity::Activity(const char* name, const Sprite* icon) 
    : name(name), icon(icon) {
    this->icon = icon;
    _display = DisplayManager::getInstance();
    _input = InputManager::getInstance();
    _activityManager = ActivityManager::getInstance();
}
