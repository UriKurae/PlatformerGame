#pragma once
#include "Module.h"

struct SDL_Texture;

class Scene2 : public Module
{
public:

	Scene2();

	// Destructor
	virtual ~Scene2();

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

	// Restart level
	bool RestartLevel();

	// Check if player won
	int CheckWin();

	// visualize all Colliders
	bool ShowColliders();

private:

	bool deadOnScene = false;

	SString folder;

	Point<float> playerStartPosition;
	SDL_Texture* sky;
	SDL_Texture* clouds;
	SDL_Texture* sea;

};

