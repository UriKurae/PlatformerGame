#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "Collider.h"
#include "Item.h"

#include "SDL_image/include/SDL_image.h"

class Player : public Entity
{
public:
	// Constructor
	Player(iPoint pos);

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
	void Draw() override;

	bool CleanUp() override;

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

	bool GetReachable() const;

	void PickItem(EntityType type);

	SDL_Rect GetSize() const;

public:

	// For the camera movement
	bool blockCamera;

	// Control if the player loaded a saved game
	bool loadedGame = false;

	// God Mode
	bool godMode = false;

	// Player saved position
	iPoint savedPosition;

	// Hearts starting position
	iPoint heartPosition;

	// Speed to run
	float speedX;
	float speedY;

	// Player's life
	int lifes = 3;

	// Collectibles info
	int gemsAchieved = 0;

private:

	// Collider deployed when player attacks
	Collider* attackCollider;

	// Direction player is facing at
	SString direction;

	// Dodging bool
	bool isDodging;
	int dodgingCooldown;
	int dodgingTime;

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
	Animation dodgingToLeft;
	Animation dodgingToRight;

	// Fx
	int jumpFx;
	int hitFx;
	int dodgingFx;
	int pickGemFx;
	int pickHeart;

	// If the player is on a reachable position or not
	bool isReachable;

};

#endif //!__MODULE_PLAYER_H__