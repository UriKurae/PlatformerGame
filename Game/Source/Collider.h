#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

class Module;

struct Collider
{
	enum TYPE
	{
		NONE = -1,
		ENEMY,
		PLAYER,

		MAX
	};

	//Methods
	Collider(SDL_Rect rectangle, TYPE type, Module* listener = nullptr);

	void SetPos(int x, int y);
	void SetH(int h);

	bool Intersects(const SDL_Rect& r) const;

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	TYPE type;
	Module* listener = nullptr;
};


#endif // !__COLLIDER_H__

