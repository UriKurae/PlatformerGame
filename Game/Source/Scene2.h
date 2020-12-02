#pragma once

#include "Scene.h"

class Player;

class Scene2 : public Scene
{
public:
	Scene2();

	// Destructor
	virtual ~Scene2();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Draw Method
	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Restart level
	bool RestartPlayerPosition();

	// Check if player won
	int CheckWin();

private:

	// Player's start position as iPoint  
	iPoint playerStartPosition;

	// Textures of the Scene2
	SDL_Texture* sky;
	SDL_Texture* clouds;
	SDL_Texture* sea;

};