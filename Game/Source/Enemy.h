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

enum class EnemyState
{
	PATROL,
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

	virtual void EnemyDies();

	virtual bool Patrol(float dt, iPoint playerPos);

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

public:

	iPoint savedPosition;
	int speedX;
	int speedY;

	int life;
	int damage;
	int attackSpeed;

	bool isAlive = true;
	bool savedIsAlive = false;

	EnemyState currentState;
	int pathCooldown;

	Animation* currentAnim;

	DynArray<iPoint> path;
};