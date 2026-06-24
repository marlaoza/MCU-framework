#ifndef UISTYLE_H
#define UISTYLE_H
#include <Arduino.h>
#include <Assets.h>
#include <Adafruit_GC9A01A.h>
#include <HalConfig.h>
#include <optional>
#include "Utils.h"

enum TextAlign {
    LEFT,
    CENTER,
    RIGHT
};


struct UIStyle {
    std::optional<int16_t> x;
    std::optional<int16_t> y;
    std::optional<int16_t> width;
    std::optional<int16_t> height;
    std::optional<int8_t> borderRadius;
    std::optional<uint16_t> color;
    std::optional<Sprite*> sprite;
    std::optional<int16_t> z;
    std::optional<int16_t> offsetX;
    std::optional<int16_t> offsetY;
    std::optional<int8_t> paddingX;
    std::optional<int8_t> paddingY;
    std::optional<uint16_t> textColor;
    std::optional<uint8_t> textSize;
    std::optional<TextAlign> textAlign;
};

struct UIAnimation {
    std::vector<UIStyle> steps;
    int curStep;
    InterpolationType interpolation;
    int stepTime;
    int duration;
    bool repeat;
    UIStyle currentStyle;
};

inline UIStyle overwriteStyle(UIStyle base, UIStyle patch) {
    if (patch.x) base.x = patch.x;
    if (patch.y) base.y = patch.y;
    if (patch.width) base.width = patch.width;
    if (patch.height) base.height = patch.height;
    if (patch.borderRadius) base.borderRadius = patch.borderRadius;
    if (patch.color) base.color = patch.color;
    if (patch.sprite) base.sprite = patch.sprite;
    if (patch.z) base.z = patch.z;
    if (patch.offsetY) base.offsetY = patch.offsetY;
    if (patch.offsetX) base.offsetX = patch.offsetX;
    if (patch.paddingY) base.paddingY = patch.paddingY;
    if (patch.paddingX) base.paddingX = patch.paddingX;
    if (patch.textColor) base.textColor = patch.textColor;
    if (patch.textSize) base.textSize = patch.textSize;
    if (patch.textAlign) base.textAlign = patch.textAlign;

    return base;
}


#endif

