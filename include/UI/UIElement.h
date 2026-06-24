#ifndef UIElement_H
#define UIElement_H

#include <map>
#include <Adafruit_GC9A01A.h>
#include "Assets.h"
#include "UIStyle.h"
#include "string"
#include "functional"
#include "vector"

enum class UIState {
    BASE, HOVERED, SELECTED, DISABLE, HIDDEN
};

#define NEIGHBOR_UP 0
#define NEIGHBOR_DOWN 1
#define NEIGHBOR_LEFT 2
#define NEIGHBOR_RIGHT 3

class UIElement {
protected:
    const char* text;
    UIElement* parent;
    std::map<std::string, UIElement*> children;
    UIElement* focusedChild;

    UIElement* neighbors[4] = {nullptr, nullptr, nullptr, nullptr};
    UIStyle stateStylePatch[3] = {{}, {}, {}};

    UIStyle style;
    UIAnimation* animation;

    UIState state;
public:
    int z;
    std::string id;
    uint8_t index;
    UIElement(std::string id, UIStyle style);

    ~UIElement();

    UIElement* getFocusedChild() {return this->focusedChild;}
    void setFocusedChild(UIElement* child);

    void setAnimation(UIAnimation anim);
    void runAnimation();

    void setState(UIState state);
    UIState getState();

    void setChildLayout(UIElement*** grid, int cols, int rows);

    void setNeighbor(int slot, UIElement* element);

    bool handleInput(uint8_t button);

    std::function<void(UIElement*)> onConfirm = nullptr;
    std::function<void(UIElement*)> onHover = nullptr;
    std::function<void(UIElement*)> offHover = nullptr;
    
    void setParent(UIElement* parent);

    UIElement* getChild(std::string id);
    virtual void addChild(UIElement* child);
    void clearChildren();
    int getChildAmount();
    std::vector<UIElement*> getChildren();

    void updateStateStyle(UIState state, const UIStyle& patch);
    void setStyle(UIStyle style);
    UIStyle getStyle();

    void setText(const char* text);
    const char* getText();

    void render(Adafruit_GFX* tft, int stripOffset = 0);
};



#endif