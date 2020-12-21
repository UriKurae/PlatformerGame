#pragma once

#include "Scene.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class SDL_Texture;

class MainMenu : public Scene
{
public:
	MainMenu();
	virtual ~MainMenu();

	bool Start();
	bool Update();
	bool CleanUp();


private:

	SDL_Texture* bg;

	// Buttons
	GuiButton* btnPlay;
	GuiButton* btnContinue;
	GuiButton* btnSettings;
	GuiButton* btnCredits;
	GuiButton* btnExit;

	// Sliders
	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFxVolume;

	// Check-Boxes
	GuiCheckBox* cbFullScreen;
	GuiCheckBox* cbVsync;


};