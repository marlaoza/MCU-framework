#include "UI/UIElement.h"
#include <HalConfig.h>
#include <vector>
#include "Managers/UIManager.h"
#include <Utils.h>



UIElement::UIElement(std::string id, UIStyle style){
    this->id = id;
    this->style = style;
    this->z = style.z.value_or(1);
    this->text = nullptr;
    this->parent = nullptr;
    this->state = UIState::BASE;
    this->focusedChild = nullptr;
    this->animation = nullptr;
}

   
UIElement::~UIElement(){
    for (std::pair<std::string, UIElement *> c : this->children){delete c.second;}
    this->children.clear();
    UIManager::getInstance()->setDirtyFlag(true);
}


void UIElement::setFocusedChild(UIElement* child) {
    if (child == nullptr) return;
    if(child->getState() == UIState::DISABLE || child->getState() == UIState::HIDDEN){return;}
    if(this->focusedChild != nullptr && this->focusedChild != child){this->focusedChild->setState(UIState::BASE); }
    child->setState(UIState::HOVERED);
    this->focusedChild = child;
}

void UIElement::setNeighbor(int slot, UIElement* element){
    this->neighbors[slot] = element;
}

bool UIElement::handleInput(uint8_t button){
    if(this->focusedChild != nullptr){
        UIState childState = this->focusedChild->getState();
        if(childState == UIState::SELECTED){
            if(this->focusedChild->handleInput(button)) {return true;}
        }else if(childState == UIState::HOVERED && button == NAV_CONFIRM){
            this->focusedChild->setState(UIState::SELECTED); return true;
        }
    
        UIElement* nextElement = nullptr;

        if(button == NAV_UP && this->focusedChild->neighbors[NEIGHBOR_UP]){nextElement = this->focusedChild->neighbors[NEIGHBOR_UP];}
        else if(button == NAV_DOWN && this->focusedChild->neighbors[NEIGHBOR_DOWN]){nextElement = this->focusedChild->neighbors[NEIGHBOR_DOWN];}
        else if(button == NAV_LEFT && this->focusedChild->neighbors[NEIGHBOR_LEFT]){nextElement = this->focusedChild->neighbors[NEIGHBOR_LEFT];}
        else if(button == NAV_RIGHT && this->focusedChild->neighbors[NEIGHBOR_RIGHT]){nextElement = this->focusedChild->neighbors[NEIGHBOR_RIGHT];}

        if(nextElement){
            this->setFocusedChild(nextElement);
            return true;
        }
    }

    if(button == NAV_CONFIRM){
        if(onConfirm){this->onConfirm(this); return true;}
        if(this->state == UIState::HOVERED){this->setState(UIState::SELECTED); return true;}
    }
    else if(button == NAV_CANCEL){
        if(this->state == UIState::SELECTED){this->setState(UIState::HOVERED); return true;}
    }

    return false;
}


void UIElement::setChildLayout(UIElement*** grid, int cols, int rows){
    for(int y = 0; y<rows; y++){
        for(int x = 0; x<cols; x++){
            UIElement* current = grid[y][x];
            if (current == nullptr) continue;
            for (int i = x + 1; i < cols; i++) {if (grid[y][i] != nullptr) { current->setNeighbor(NEIGHBOR_RIGHT, grid[y][i]); break; }}
            for (int i = x - 1; i >= 0; i--) {if (grid[y][i] != nullptr) { current->setNeighbor(NEIGHBOR_LEFT, grid[y][i]); break; }}
            for (int j = y + 1; j < rows; j++) {if (grid[j][x] != nullptr) { current->setNeighbor(NEIGHBOR_DOWN, grid[j][x]); break; }}
            for (int j = y - 1; j >= 0; j--) {if (grid[j][x] != nullptr) {current->setNeighbor(NEIGHBOR_UP, grid[j][x]); break; }}
            this->addChild(current);
        }
    }
}


int UIElement::getChildAmount(){
    return this->children.size();
}

std::vector<UIElement*> UIElement::getChildren() {
    std::vector<UIElement*> childrenVec;
    for (const std::pair<std::string, UIElement *>& p : this->children) {
        childrenVec.push_back(p.second);
    }

    return childrenVec;
}

void UIElement::setText(const char* text){
    this->text = text;
    UIManager::getInstance()->setDirtyFlag(true);
}

void UIElement::setState(UIState state) {
    if(state == UIState::HOVERED && onHover){onHover(this);}
    if(this->state == UIState::HOVERED && state == UIState::BASE && offHover){offHover(this);}
    this->state = state;
    this->z = getStyle().z.value_or(1);
    UIManager::getInstance()->setDirtyFlag(true);
}

UIState UIElement::getState(){return this->state;}


UIElement* UIElement::getChild(std::string id){return this->children[id];}

void UIElement::addChild(UIElement* child){
    if(this->children[child->id] == nullptr){
        child->setParent(this);
        this->children[child->id] = child;
        child->index = this->children.size() - 1;
        UIManager::getInstance()->setDirtyFlag(true);
    }
}

const char* UIElement::getText(){
    return this->text;
}


void UIElement::setParent(UIElement* parent){
    this->parent = parent;
}

void UIElement::updateStateStyle(UIState state, const UIStyle& patch){
    int stateInt = (int)state;
    if(stateInt > 3) {return;}
    UIStyle defaultPatch = this->style;
    if(stateInt > 0) defaultPatch = stateStylePatch[stateInt - 1];
    defaultPatch = overwriteStyle(defaultPatch, patch);
    if(stateInt > 0) stateStylePatch[stateInt - 1] = defaultPatch;
    else this->style = defaultPatch;
    
    this->z = getStyle().z.value_or(1);
 
}
void UIElement::setStyle(UIStyle style){this->style = style; this->z = getStyle().z.value_or(1); UIManager::getInstance()->setDirtyFlag(true);}
UIStyle UIElement::getStyle(){
    UIStyle retStyle = this->style;
    if(this->animation != nullptr){
        retStyle = overwriteStyle(retStyle, this->animation->currentStyle);
    }
    int stateInt = (int)this->state;
    if(stateInt > 0 && stateInt <= 3){
        UIStyle patch = stateStylePatch[stateInt - 1];
        retStyle = overwriteStyle(retStyle, patch);
    }

    if (!retStyle.x) retStyle.x = 0;
    if (!retStyle.y) retStyle.y = 0;
    if (!retStyle.width) retStyle.width = 0;
    if (!retStyle.height) retStyle.height = 0;
    if (!retStyle.borderRadius) retStyle.borderRadius = 0;
    if (!retStyle.borderSize) retStyle.borderSize = 0;
    if (!retStyle.borderColor) retStyle.borderColor = GC9A01A_BLACK;
    if (!retStyle.color) retStyle.color = GC9A01A_BLACK;
    if (!retStyle.z) retStyle.z = 1;
    if (!retStyle.offsetY) retStyle.offsetY = 0;
    if (!retStyle.offsetX) retStyle.offsetX = 0;
    if (!retStyle.paddingY) retStyle.paddingY = 0;
    if (!retStyle.paddingX) retStyle.paddingX = 0;
    if (!retStyle.textColor) retStyle.textColor = GC9A01A_WHITE;
    if (!retStyle.textSize) retStyle.textSize = 1;
    if (!retStyle.textAlign) retStyle.textAlign = TextAlign::LEFT;
    if (!retStyle.sprite) retStyle.sprite = nullptr;

    return retStyle;
}

void UIElement::clearChildren(){
    this->children.clear();
};

void UIElement::setAnimation(UIAnimation anim){
    if(this->animation) delete this->animation;
    this->animation = new UIAnimation(anim);
    this->animation->stepTime = 0;
    this->animation->curStep = 0;
    this->animation->currentStyle = this->animation->steps[0];
};
void UIElement::runAnimation(){
    for (const std::pair<std::string, UIElement *>& p : this->children) {
        p.second->runAnimation();
    }

    if(this->animation == nullptr) {return;}

    if(this->animation->stepTime <= 0){
        this->animation->stepTime = millis();
    }
    int delta =  millis() - this->animation->stepTime;

    float stepDuration = this->animation->duration / this->animation->steps.size();
    if(delta >= stepDuration){
        if(this->animation->curStep >= (int)this->animation->steps.size()-2){
            if(this->animation->repeat){
                this->animation->curStep = 0;
                this->animation->stepTime = millis();
            }
            else{this->animation = nullptr;}
            return;
        }else{
            this->animation->curStep +=1;
            this->animation->stepTime = millis();
            delta = 0;
        }
    }
    float deltaInterpolation = (float)delta/stepDuration;

    UIStyle start = this->animation->steps[this->animation->curStep];
    UIStyle end = this->animation->steps[this->animation->curStep + 1];
    UIStyle retStyle = start; 
    
    if (end.width) retStyle.width = start.width ? interpolate(start.width.value(), end.width.value(), deltaInterpolation, this->animation->interpolation) : end.width.value();
    if (end.height) retStyle.height = start.height ? interpolate(start.height.value(), end.height.value(), deltaInterpolation, this->animation->interpolation) : end.height.value();
    if (end.borderRadius) retStyle.borderRadius = start.borderRadius ? interpolate(start.borderRadius.value(), end.borderRadius.value(), deltaInterpolation, this->animation->interpolation) : end.borderRadius.value();
    if (end.offsetX) retStyle.offsetX = start.offsetX ? interpolate(start.offsetX.value(), end.offsetX.value(), deltaInterpolation, this->animation->interpolation) : end.offsetX.value();
    if (end.offsetY) retStyle.offsetY = start.offsetY ? interpolate(start.offsetY.value(), end.offsetY.value(), deltaInterpolation, this->animation->interpolation) : end.offsetY.value();
    if (end.paddingX) retStyle.paddingX = start.paddingX ? interpolate(start.paddingX.value(), end.paddingX.value(), deltaInterpolation, this->animation->interpolation) : end.paddingX.value();
    if (end.paddingY) retStyle.paddingY = start.paddingY ? interpolate(start.paddingY.value(), end.paddingY.value(), deltaInterpolation, this->animation->interpolation) : end.paddingY.value();
    if (end.textSize) retStyle.textSize = start.textSize ? interpolate(start.textSize.value(), end.textSize.value(), deltaInterpolation, this->animation->interpolation) : end.textSize.value();
    if (end.x) retStyle.x = start.x ? interpolate(start.x.value(), end.x.value(), deltaInterpolation, this->animation->interpolation) : end.x.value();
    if (end.y) retStyle.y = start.y ? interpolate(start.y.value(), end.y.value(), deltaInterpolation, this->animation->interpolation) : end.y.value();
    
    if (end.borderSize) retStyle.borderSize = start.borderSize ? interpolate(start.borderSize.value(), end.borderSize.value(), deltaInterpolation, this->animation->interpolation) : end.borderSize.value();
    
    if (end.color) retStyle.color = start.color ? interpolateColor(start.color.value(), end.color.value(), deltaInterpolation, this->animation->interpolation) : end.color.value();
    if (end.textColor) retStyle.textColor = start.textColor ? interpolateColor(start.textColor.value(), end.textColor.value(), deltaInterpolation, this->animation->interpolation) : end.textColor.value();
    if (end.borderColor) retStyle.borderColor = start.borderColor ? interpolateColor(start.borderColor.value(), end.borderColor.value(), deltaInterpolation, this->animation->interpolation) : end.borderColor.value();

    if (end.sprite) retStyle.sprite = end.sprite;
    if (end.z) retStyle.z = end.z;
    if (end.textAlign) retStyle.textAlign = end.textAlign;

    this->animation->currentStyle = retStyle;

    UIManager::getInstance()->setDirtyFlag(true);
    
};

void UIElement::render(Adafruit_GFX* tft, int stripOffset){
    if(this->state == UIState::HIDDEN){return;}

    UIStyle s = this->getStyle();
    int16_t renderX = s.x.value_or(0) + s.offsetX.value_or(0);
    int16_t renderY = s.y.value_or(0) + s.offsetY.value_or(0) - stripOffset;

    if(this->parent != nullptr){
        UIStyle parentS = this->parent->getStyle();
        renderX += parentS.x.value_or(0) + parentS.offsetX.value_or(0) + parentS.paddingX.value_or(0);
        renderY += parentS.y.value_or(0) + parentS.offsetY.value_or(0) + parentS.paddingY.value_or(0);
    }
   
    tft->fillRoundRect(renderX, renderY, s.width.value_or(0), s.height.value_or(0), s.borderRadius.value_or(0), s.color.value_or(GC9A01A_BLACK));
    if(s.borderSize.value() > 0){
        for(int i = 0; i < s.borderSize.value(); i++){
            tft->drawRoundRect(renderX + i, renderY + i, s.width.value_or(0) - 2*i, s.height.value_or(0) - 2*i, s.borderRadius.value_or(0), s.borderColor.value_or(GC9A01A_BLACK));
        }
    }
    // if(s.sprite && s.sprite.value() != nullptr){
    //     if(s.sprite.value()->mask != nullptr){tft->drawRGBBitmap(renderX, renderY, s.sprite.value()->sprite, s.sprite.value()->mask, s.width.value_or(0), s.height.value_or(0));}
    //     else{tft->drawRGBBitmap(renderX, renderY, s.sprite.value()->sprite, s.width.value_or(0), s.height.value_or(0));}
    // }
    if(this->text != nullptr){
        tft->setTextSize(s.textSize.value_or(1));
        tft->setTextColor(s.textColor.value_or(GC9A01A_WHITE));
        tft->setCursor(renderX + s.paddingX.value_or(0), renderY + s.paddingY.value_or(0));

        if(s.textAlign == TextAlign::CENTER){
            int16_t x1, y1;
            uint16_t w, h;
            tft->getTextBounds(this->text, 0, 0, &x1, &y1, &w, &h);
            int16_t tx = renderX + (s.width.value_or(0)  - w) / 2 - x1;
            tft->setCursor(tx, renderY + s.paddingY.value_or(0));
        }
        if(s.textAlign == TextAlign::RIGHT){
            tft->setCursor(s.width.value_or(0) - s.paddingX.value_or(0), renderY + s.paddingY.value_or(0));
        }

        tft->print(this->text);
    }


    std::vector<std::pair<std::string, UIElement *>> pairs(this->children.begin(), this->children.end());

    std::sort(pairs.begin(), pairs.end(), [](const std::pair<std::string, UIElement *>& a, const std::pair<std::string, UIElement *>& b) {
        return a.second->z < b.second->z;
    });

    for (const std::pair<std::string, UIElement *>& p : pairs) {
        p.second->render(tft, stripOffset);
    }

}