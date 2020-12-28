#pragma once

#include "Scene.h"
#include "Animation.h"

class Player;
class Executioner;
class Wolf;
class GreenGem;
class RedHeart;

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

	// Draw GUI
	void DrawGui();

	// Called before quitting
	bool CleanUp();

	// Check if player won
	int CheckWin();

	// Detect any mouse event while the game is paused
	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:

	Player* player;

	// Texture for Gui
	SDL_Texture* guiTexture;

	// Player's start position as iPoint  
	iPoint playerStartPosition;

	// Textures of the Scene2
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
	List<iPoint> checkpoints;
	ListItem<iPoint>* checkpointItem;
	Animation* currentAnimCheckpoint;

	// Gui font index
	int uiIndex;

	// timer
	float timer;
	char timerText[3] = { "\0" };

	// Animation for UI Timer
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