#pragma once

#include "Point.h"
#include "Animation.h"
#include "Collider.h"

#include "SDL_image/include/SDL_image.h"

class Player;

enum class EnemyType
{
	NONE = -1,
	EXECUTIONER,
	BIRD,
	GROUND,
};

class Enemy
{
public:
	Enemy(iPoint pos);
	Enemy() {};

	virtual ~Enemy();

	
	virtual bool Update(float dt);

	virtual void Draw();

	virtual bool CleanUp();
	
	virtual bool FindTarget(Player* player);

	virtual bool ChaseTarget(iPoint position);

public:
	iPoint position;
	int life;
	int damage;
	int attackSpeed;

	SDL_Texture* texture;
	Animation* currentAnim;
	Collider* collider;

};
