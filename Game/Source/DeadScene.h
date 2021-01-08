#pragma once

#include "Scene.h"
#include "GuiButton.h"

class SDL_Texture;

class DeadScene : public Scene
{
public:

	DeadScene();

	// Destructor
	virtual ~DeadScene();

	// Called before the first frame
	bool Start() override;

	// Called each loop iteration
	bool Update(float dt) override;

	bool Draw() override;

	// Called before quitting
	bool CleanUp() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

public:

	Module* lastScene;

private:

	SDL_Texture* deadTexture;
	SDL_Texture* playerTexture;

	GuiButton* btnRestartLevel;
	GuiButton* btnBackToMenu;

	Animation* currentPlayerAnim;
	Animation playerKneelDown;
	Animation playerOnKnees;
};