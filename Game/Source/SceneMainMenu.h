#pragma once

#include "Scene.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class MainMenu : public Scene
{
public:
	//Constructor
	MainMenu();

	//Destructor
	~MainMenu();

	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool Draw() override;

	bool CleanUp() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:

	SDL_Texture* intro;
	SDL_Texture* logo;

	float count;
	bool showLogo = true;

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

