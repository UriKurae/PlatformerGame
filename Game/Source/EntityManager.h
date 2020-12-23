#pragma once

#include "Module.h"

#include "Enemy.h"
#include "Item.h"

#include "List.h"
#include "Point.h"
#include "Entity.h"

class EntityManager : public Module
{
public:
	EntityManager();

	bool Start();
	Entity* CreateEntity(EntityType type, iPoint pos);
	void DeleteEntity(Entity* entity);
	bool Update(float dt);
	bool Draw();
	bool CleanUp();
	
public:

	List<Entity*> entities;
	List<Enemy*> enemies;
	List<Item*> items;
	SString name;
	SDL_Texture* executionerTexture;
	SDL_Texture* wolfTexture;
};