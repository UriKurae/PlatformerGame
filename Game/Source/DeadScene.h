#pragma once

#include "Scene.h"

struct SDL_Texture;

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

public:

	Module* lastScene;

private:

	SDL_Texture* deadTexture;
};