#pragma once

#include "Scene.h"

class Player;
class Executioner;
class Wolf;
class GreenGem;
class RedHeart;
class Animation;


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

	// Enemies
	Executioner* executioner;
	Executioner* executioner2;
	Executioner* executioner3;
	Wolf* wolf;
	Wolf* wolf2;
	Wolf* wolf3;
	Wolf* wolf4;

	// Collectibles
	GreenGem* gem1;
	GreenGem* gem2;
	GreenGem* gem3;
	GreenGem* gem4;

	RedHeart* heart1;
	RedHeart* heart2;
	RedHeart* heart3;

	// CheckPoints
	bool checkpoint1 = false;
	bool checkpoint2 = false;
	bool checkSound1 = false;
	bool checkSound2 = false;
	Animation* currentAnim;

};