#pragma once
#include "SDL/include/SDL_Rect.h"

class Module;

struct Collider
{
	enum Type
	{
		NONE = -1,
		WALL,
		PLAYER,
		PLAYER_SHOT,
		ENEMY,
		BALLOON,
		BREAKABLE_BLOCK,
		UNBREAKABLE_BLOCK,
		STAIR,
		BOOSTERS,

		MAX
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);
	void SetH(int h);

	bool Intersects(const SDL_Rect& r) const;

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
};
