#pragma once

#include "Entity.h"

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

class Enemy : public Entity
{
public:
	Enemy(iPoint pos);

	virtual ~Enemy();

	bool Start() override;
	
	bool Update(float dt) override;

	void Draw() override;

	bool CleanUp() override;
	
	virtual void HandleCollisions(float dt);

	virtual bool FindTarget(Player* player, float dt);

	virtual bool ChaseTarget(float dt);

	virtual void TakeDamage(int damage);
	
	virtual void Attack();

	virtual void EnemyDies();

	virtual bool Patrol(float dt, iPoint playerPos);

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

	Animation* currentAnim;
	Collider* collider;

	DynArray<iPoint> path;

};
