#pragma once

#include "Module.h"

struct SDL_Texture;

class WinScene : public Module
{
public:

	WinScene();

	// Destructor
	virtual ~WinScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config) override;

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


private:

	SDL_Texture* winTexture;
};