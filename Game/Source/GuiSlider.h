#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiSlider();

    bool Update(Input* input, float dt, iPoint position);
    bool Draw(Render* render, bool debugDraw);
    void CalculateValue();
    int GetValue() const;

private:

    SDL_Rect slider;
    int value;

    int minValue;
    int maxValue;

    iPoint initialPos;
    int mouseX, mouseY;

    // Fx's
    int fxMouseRelease;
    int fxMouseClick;
    int fxMouseHover;

    bool mouseFxDone = false;
};

#endif // __GUISLIDER_H__