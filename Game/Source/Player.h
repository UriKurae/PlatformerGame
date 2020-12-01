#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Collider.h"

struct SDL_Texture;

class Player : public Module
{
public:
	// Constructor
	Player();

	// Destructor
	virtual ~Player();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start();

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	void Draw();

	bool CleanUp();

	void CameraFollow();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&);

	iPoint SetPosition(int x, int y);
	iPoint GetPosition();

	// Collisions logic
	void OnCollision();

	void Jump(float dt);

	void LoadPushbacks();

public:

	// Player's current level
	int currentLevel = 0;

	// Control if the player loaded a saved game
	bool loadedGame = false;

	// God Mode
	bool godMode = false;


private:

	Collider* collider;

	//float delt = 0;

	// Position of player
	iPoint position;

	// Direction player is facing at
	SString direction;

	// Speed to run
	float speedX;
	float speedY;

	// Camera offset
	float camOffset = position.x;

	// Constant gravity applied to the player
	float gravity;

	// Jump mechanic
	bool jump = false;
	bool upwards = false;
	bool isFalling = false;
	int jumpsLeft = 0;

	// Block movements
	bool blockLeftMovement = false;
	bool blockRightMovement = false;
	bool blockJump = false;
	bool blockFall = false;

	// Texture for the player
	SDL_Texture* texture;
	
	
	// Animations
	Animation* currentAnim;
	Animation idleRightAnim;
	Animation idleLeftAnim;
	Animation runRightAnim;
	Animation runLeftAnim;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;
	Animation fallingRightAnim;
	Animation fallingLeftAnim;
	
};

#endif //!__MODULE_PLAYER_H__