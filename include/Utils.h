#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"
#include <random>

inline String generateRandomString(int length){
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    String result = "";
    
    for (size_t i = 0; i < length; i++) {
        uint32_t randVal = esp_random(); 
        int index = randVal % (sizeof(charset) - 1);
        result += charset[index];
    }
    return result;
}

//LOG
#define LOGLEVEL_SYSTEM "SYSTEM"
#define LOGLEVEL_UI "UI"
#define LOGLEVEL_INPUT "INPUT"

#define LOGSTATUS_INFO "INFO"
#define LOGSTATUS_WARNING "WARNING"
#define LOGSTATUS_ERROR "ERROR"
#define LOGSTATUS_SUCCESS "SUCCESS"

inline void sysLog(const char* level, const char* status, const char* msg, const char* args = ""){
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "[%s | %s]: %s", level, status, msg);
    Serial.print(buffer);
    
    if (args[0] != '\0') {
        Serial.print(":\n");
        Serial.print(args);
    }
    
    Serial.println();
}

inline void sysLog(const char* level, const char* status, const char* msg, uint args){
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "[%s | %s]: %s", level, status, msg);
    Serial.print(buffer);
    Serial.print(":\n");
    Serial.print(args);
    
    Serial.println();
}

enum InterpolationType {
    Linear,
    EaseIn,
    EaseOut,
    Snap,
};

template <typename T>
inline T interpolate(T start, T end, float t, InterpolationType type) {
    if(type == Snap){return end;}
    if(type == EaseIn){t*=t;}
    if(type == EaseOut){t*=((float)2 - t);}
    return start + (T)((float)(end - start) * t);
}

inline uint16_t interpolateColor(uint16_t start, uint16_t end, float t, InterpolationType type) {

    uint8_t r1 = (start >> 11) & 0x1F;
    uint8_t g1 = (start >> 5) & 0x3F;
    uint8_t b1 = start & 0x1F;

    uint8_t r2 = (end >> 11) & 0x1F;
    uint8_t g2 = (end >> 5) & 0x3F;
    uint8_t b2 = end & 0x1F;

    uint8_t r = interpolate(r1, r2, t, type);
    uint8_t g = interpolate(g1, g2, t, type);
    uint8_t b = interpolate(b1, b2, t, type);

    return ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
}


#endif