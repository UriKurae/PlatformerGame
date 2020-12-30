#pragma once

#include "Scene.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class SDL_Texture;
enum class MenuState;

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

	void SetContinueButton(GuiControlState state);

	bool Load();

private:

	SDL_Texture* intro;
	SDL_Texture* textureCredits;

	// Principal GUI
	GuiButton* btnPlay;
	GuiButton* btnContinue;
	GuiButton* btnSettings;
	GuiButton* btnCredits;
	GuiButton* btnExit;

	// Options GUI
	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFxVolume;
	GuiCheckBox* fullScreenCheckBox;
	GuiCheckBox* vSyncCheckBox;
	GuiButton* btnBackOptions;

	// Credits GUI
	GuiButton* btnBackCredits;

	// Current menu state
	MenuState menuState;

	// UI Fx
	int fxClickButton;

};