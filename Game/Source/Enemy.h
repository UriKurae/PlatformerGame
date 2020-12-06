#pragma once

#include "Point.h"
#include "Animation.h"
#include "Collider.h"
#include "SString.h"
#include "DynArray.h"
#include "List.h"
#include "PQueue.h"

#include "SDL_image/include/SDL_image.h"
#include "PugiXml/src/pugixml.hpp"


class Player;

enum class EnemyType
{
	NONE = -1,
	EXECUTIONER,
	BIRD,
	WOLF,
};

enum class EnemyState
{
	PATROL = 0,
	ALERT,
	ATTACK,
};

class Enemy
{
public:
	Enemy(iPoint pos);
	Enemy() {};

	virtual ~Enemy();

	virtual bool Start();
	
	virtual bool Update(float dt);

	virtual void Draw();

	virtual bool CleanUp();
	
	virtual void HandleCollisions(float dt);

	virtual bool FindTarget(Player* player, float dt);

	virtual bool ChaseTarget(float dt);

	virtual void TakeDamage(int damage);

	virtual void EnemyDies();

	virtual bool Patrol(float dt);

	// Load and save functions for each enemy
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}
	virtual bool Save(pugi::xml_node&)
	{
		return true;
	}

public:
	SString name;

	iPoint position;
	iPoint savedPosition;
	int speedX;
	int speedY;

	int life;
	int damage;
	int attackSpeed;

	bool isAlive;
	bool savedIsAlive;

	EnemyState currentState;
	int pathCooldown;

	SDL_Texture* texture;
	Animation* currentAnim;
	Collider* collider;

	DynArray<iPoint> path;

};
