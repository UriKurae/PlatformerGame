#pragma once

#include "Point.h"
#include "Animation.h"
#include "Collider.h"
#include "SString.h"

#include "SDL_image/include/SDL_image.h"
#include "PugiXml/src/pugixml.hpp"


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

	virtual bool ChaseTarget();

	virtual void TakeDamage(int damage);

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
	int life;
	int damage;
	int attackSpeed;

	SDL_Texture* texture;
	Animation* currentAnim;
	Collider* collider;

};
