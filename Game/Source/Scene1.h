#pragma once

#include "Scene.h"
#include "Animation.h"

class Player;
class Executioner;
class Wolf;
class GreenGem;
class RedHeart;

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

	// Draw GUI
	void DrawGui();

	// Called before quitting
	bool CleanUp();

	// Restart level
	bool RestartPlayerPosition();

	// Check if player won
	int CheckWin();

	// Pro way function
	bool CheckCollisions(SDL_Rect& a, SDL_Rect& b) override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:

	// Texture for Gui
	SDL_Texture* guiTexture;

	// Player object
	Player* player;

	// Player's start position as iPoint  
	iPoint playerStartPosition;

	// Textures of the Scene1
	SDL_Texture* sky;
	SDL_Texture* clouds;
	SDL_Texture* sea;

	// Enemies
	List<Wolf*> wolfs;
	List<Executioner*> executioners;

	// Collectibles
	List<GreenGem*> gems;
	List<RedHeart*> hearts;

	// Checkpoints
	bool checkpoint1 = false;
	bool checkpoint2 = false;
	bool checkSound1 = false;
	bool checkSound2 = false;

	Animation* currentAnimCheckpoint;

	// Gui font index
	int uiIndex;
	
	// timer
	float timer;
	char timerText[3] = { "\0" };

	// Animation for UI Timer
	SDL_Texture* timerTexture;
	Animation* currentAnimTimer;
	Animation timerAnimation;

	// Animation for UI heart
	SDL_Texture* heartTexture;
	Animation* currentAnimHeart;
	Animation heartAnimation;

	// Animation for UI heart
	SDL_Texture* gemTexture;
	Animation* currentAnimGem;
	Animation gemAnimation;

};