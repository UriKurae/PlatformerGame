#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Animation.h"
#include "Point.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char *text);
	virtual ~GuiButton();

	bool Update(Input* input, float dt, iPoint position);
	bool Draw(Render* render, bool debugDraw);

private:

	// Gui Button specific properties
	// Maybe some animation properties for state change?
	Animation* currentAnim;
	Animation animButtonHighlighted;
	Animation animButtonPressed;
	Animation animButtonMouseInside;

	// Fx
	int fxMouseClick;
	int fxMouseHover;

	bool mouseFxDone = false;
};

#endif // __GUIBUTTON_H__