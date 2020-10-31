#pragma once

#include "Module.h"

struct SDL_Texture;

class DeadScene : public Module
{
public:

	DeadScene();

	// Destructor
	virtual ~DeadScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config) override;

	// Called before the first frame
	bool Start() override;

	// Called before all Updates
	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;


private:

	SDL_Texture* deadTexture;
};