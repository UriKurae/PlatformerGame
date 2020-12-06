#pragma once

#include "SString.h"
#include "Point.h"

class SDL_Texture;
class Animation;
class Collider;

enum class ItemType
{
	NONE = -1,
	GEM,
	COIN,
	HEART,
};

class Item
{
public:

	Item(iPoint position);
	virtual ~Item();

	virtual bool Start();
	
	virtual bool Update(float dt);
	
	virtual void Draw();
	
	virtual bool CleanUp();

	virtual ItemType GetType();

public:
	SString name;
	iPoint position;
	ItemType type;

	SDL_Texture* texture;
	Animation* currentAnim;
	Collider* collider;

	bool active;
};
