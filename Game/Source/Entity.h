#pragma once

#include "Point.h"
#include "SString.h"

class SDL_Texture;
class Collider;

enum class EntityType
{
	PLAYER,
	EXECUTIONER,
	WOLF,
	GEM,
	HEART,
	UNKNOWN
};

class Entity
{
public:
	Entity(iPoint pos, bool isActive = true) : position(pos) {};

	virtual bool Start() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual void Draw() {};
	virtual bool CleanUp() { return true; };
	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) { return true; }

	virtual void DisableEntity()
	{
		if (active == true)
		{
			active = false;
			CleanUp();
		}
	};
	virtual void EnableEntity()
	{
		if (active == false)
		{
			active = true;
			Start();
		}
	};


public:
	bool active;
	SString name;

	iPoint position;
	EntityType type;
	SDL_Texture* texture;
	Collider* collider;
};