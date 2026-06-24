#include "Managers/InputManager.h"
#include "HalConfig.h"
#include "Utils.h"


uint8_t buttons[] = {BTN_26, BTN_25, BTN_35, BTN_27, BTN_16, BTN_17};

InputManager* InputManager::_instance = nullptr;

InputManager* InputManager::getInstance() {
    if (_instance == nullptr) _instance = new InputManager();
    return _instance;
}

bool InputManager::begin() {
    int c = 0;
    for(uint8_t btn : buttons) {
        pinMode(btn, INPUT_PULLDOWN);
        _btnStates[c] = false;
        c++;
    }

    _inputQueue = xQueueCreate(10, sizeof(uint8_t));
    if (_inputQueue == NULL) {return false;}

    xTaskCreatePinnedToCore(InputManager::inputTask,"InputTask",4096,this,2,NULL,0);

    return true;
}

void InputManager::inputTask(void* params){
    InputManager* manager = static_cast<InputManager*>(params);
    while(true) {
        manager->handleInput();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void InputManager::handleInput() {
    for (int i = 0; i < 6; i++) {
        bool state = digitalRead(buttons[i]);
        if(state == HIGH && _btnStates[i] == LOW) {
            sysLog(LOGLEVEL_INPUT, LOGSTATUS_INFO, "Input recebido", buttons[i]);
            xQueueSend(_inputQueue, &buttons[i], 0);
        }
        _btnStates[i] = state;
    }

}


bool InputManager::popInput(uint8_t* outButtonPin) {
    if (_inputQueue == NULL) return false;
    if (xQueueReceive(_inputQueue, outButtonPin, 0) == pdTRUE) {
        return true;
    }
    return false;
}

bool InputManager::isPressed(uint8_t pin) {
    return digitalRead(pin);
}
