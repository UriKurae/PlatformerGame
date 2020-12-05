#pragma once

#include "Module.h"

struct SDL_Texture;

class WinScene : public Scene
{
public:

	WinScene();

	// Destructor
	virtual ~WinScene();

	// Called before the first frame
	bool Start() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool Draw() override;

	// Called before quitting
	bool CleanUp() override;

private:

	SDL_Texture* winTexture;
};