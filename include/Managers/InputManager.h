#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>
#include "HalConfig.h"

struct MPUData {
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
};

extern uint8_t buttons[];

class InputManager {
private:
    static InputManager* _instance;
    bool _btnStates[6]; 
    QueueHandle_t _inputQueue;
    static void inputTask(void *params);

public:
    static InputManager* getInstance();
    bool begin();
    void handleInput();
    bool isPressed(uint8_t pin);
    bool popInput(uint8_t* outButtonPin);
};

#endif