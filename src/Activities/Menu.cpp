#include "Activities/Menu.h"
#include "Managers/ActivityManager.h"
#include "Assets.h"
#include <Utils.h>

Menu::Menu() 
    : Activity("menu", nullptr) {}
void Menu::setup() {

    UIElement* screen1 = new UIElement("screen", {.x = 0, .y = 0, .width = _display->getWidth(), .height = _display->getHeight(), .color = GC9A01A_BLACK, .z = 0});
    this->screens["screen"] = screen1;
    
    UIElement* box1 = new UIElement("box1", {.x = 70,  .y = 100, .width = 50, .height = 50, .borderColor=GC9A01A_RED, .borderSize = 3, .color=GC9A01A_CYAN});
    UIElement* box2 = new UIElement("box2", {.x = 140, .y = 100, .width = 50, .height = 50, .borderColor=GC9A01A_RED, .borderSize = 1, .color=GC9A01A_CYAN});
    
    UIAnimation test = {
        .steps = {
            {.x = 70, .y = 100, .color = GC9A01A_CYAN}, 
            {.x = 65, .y = 150, .color = GC9A01A_RED},
            {.x = 70, .y = 100, .color = GC9A01A_CYAN},
            },
        .interpolation = InterpolationType::Linear,
        .duration = 800,
        .repeat = true
    };

    box1->updateStateStyle(UIState::HOVERED, {.color = GC9A01A_BLUE});
    box2->updateStateStyle(UIState::HOVERED, {.color = GC9A01A_BLUE});

    box1->updateStateStyle(UIState::SELECTED, {.color = GC9A01A_RED});
    box2->updateStateStyle(UIState::SELECTED, {.color = GC9A01A_RED});
    
    box1->setNeighbor(NEIGHBOR_RIGHT, box2);
    box2->setNeighbor(NEIGHBOR_LEFT, box1);

    box1->setAnimation(test);

    screen1->addChild(box1);
    screen1->addChild(box2);

    _screen->changeScreen(screen1);
}

void Menu::loop() {
    _screen->update();
}

void Menu::stop() {
    for (std::pair<std::string, UIElement *> c : this->screens){delete c.second;}
    this->screens.clear();
}