#pragma once

#include "Entity.h"

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

class Item : public Entity
{
public:

	Item(iPoint position);
	virtual ~Item();

	virtual bool Start() override;
	
	virtual bool Update(float dt) override;
	
	virtual void Draw() override;
	
	virtual bool CleanUp() override;

public:
	SString name;
	iPoint position;
	ItemType type;

	SDL_Texture* texture;
	Animation* currentAnim;
	Collider* collider;

	bool active;
};
