#pragma once

#include "Scene.h"

class Player;
class Executioner;
class Wolf;
class GreenGem;
class RedHeart;
class Animation;

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

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:

	Player* player;

	// Player's start position as iPoint  
	iPoint playerStartPosition;

	// Textures of the Scene2
	SDL_Texture* sky;
	SDL_Texture* clouds;
	SDL_Texture* sea;

	// Checkpoints
	bool checkpoint1 = false;
	bool checkpoint2 = false;
	bool checkSound1 = false;
	bool checkSound2 = false;
	Animation* currentAnim;

	// Enemies
	List<Wolf*> wolfs;
	List<Executioner*> executioners;

	// Collectibles
	List<GreenGem*> gems;
	List<RedHeart*> hearts;

	int uiIndex;
};