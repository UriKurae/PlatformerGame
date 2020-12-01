#pragma once

#include "Scene.h"

class Player;
class Executioner;
class Wolf;

class Scene1: public Scene
{
public:

	Scene1();

	// Destructor
	virtual ~Scene1();

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

	// Textures of the Scene1
	SDL_Texture* sky;
	SDL_Texture* clouds;
	SDL_Texture* sea;

	Executioner* executioner;
	Wolf* wolf;

};