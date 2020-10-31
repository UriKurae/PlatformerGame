#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

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
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate() override;

	bool CleanUp() override;

	bool Load(pugi::xml_node&) override;

	bool Save(pugi::xml_node&) override;

	Point<float> SetPosition(float x, float y);
	Point<float> GetPosition();

	void OnCollision();

	void Jump();

	void LoadPushbacks();

public:

private:

	// God Mode
	bool godMode = false;

	// Position of player
	Point<float> position;

	// Speed to run
	float speedX;
	float speedY;
	float minSpeedY;
	
	// Camera offset
	float camOffset = position.x;

	// Constant gravity applied to the player
	float gravity = 0.4f;

	// Jump mechanic
	bool jump = false;
	bool upwards = false;

	// Block movements
	bool blockLeftMovement = false;
	bool blockRightMovement = false;
	bool blockJump = false;
	bool blockFall = false;

	// Texture for the player
	SDL_Texture* texture;
	
	
	// Animations
	Animation* currentAnim;
	Animation idleAnim;
	Animation runRightAnim;
	Animation runLeftAnim;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;
};

#endif //!__MODULE_PLAYER_H__