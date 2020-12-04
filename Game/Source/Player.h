#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Collider.h"
#include "Item.h"

#include "SDL_image/include/SDL_image.h"

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

	void HandleInput(float dt);

	void CameraFollow();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	iPoint SetPosition(int x, int y);
	iPoint GetPosition();

	// Collisions logic
	void OnCollision();

	void Jump(float dt);

	void Attack();

	Collider* GetCollider();

	void PickItem(ItemType type);

public:

	// Control if the player loaded a saved game
	bool loadedGame = false;

	// God Mode
	bool godMode = false;

	iPoint savedPosition;

private:

	// Texture for the player
	SDL_Texture* texture;

	Collider* collider;
	Collider* attackCollider;

	// Position of player
	iPoint position;

	// Direction player is facing at
	SString direction;

	// Player's life
	int health = 3;

	// Speed to run
	float speedX;
	float speedY;

	// Constant gravity applied to the player
	float gravity;

	// Camera offset
	float camOffset = position.x;

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
	Animation attackRightDownUpAnim;
	Animation attackLeftDownUpAnim;

	// Health animation and texture
	SDL_Texture* healthTexture;
	Animation* currentAnimHeart;
	Animation hearthAnim;

	// Fx
	int jumpFx;

	// Collectibles info
	int gemsAchieved = 0;
};

#endif //!__MODULE_PLAYER_H__