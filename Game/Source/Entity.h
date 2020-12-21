#pragma once

class SDL_Texture;
class Collider;

enum class EntityType
{
	EXECUTIONER,
	WOLF,
	COINS,
	LIFE,
	UNKNOWN
};

class Entity
{
public:
	Entity(iPoint pos) : position(pos) {};

	virtual bool Start() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual void Draw() {};
	virtual bool CleanUp() { return true; };

public:
	iPoint position;
	EntityType type;
	SDL_Texture* texture;
	SString name;
	Collider* collider;
};