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
	bool Update(float dt);
	bool Draw();
	bool CleanUp();

	bool Save(pugi::xml_node&) override;
	bool Load(pugi::xml_node&) override;

	Entity* CreateEntity(EntityType type, iPoint pos);
	void DeleteEntity(Entity* entity);
	void ClearLists();

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	
	// Deletes all the colliders
	void DeleteColliders();

	void DeleteEntities();

	void RemoveCollider(Collider* c);

	void DrawColliders();
	
public:

	List<Entity*> entities;
	List<Enemy*> enemies;
	List<Item*> items;
	List<Collider*> colliders;

	SDL_Texture* executionerTexture;
	SDL_Texture* wolfTexture;
	SDL_Texture* collectiblesTexture;

	uint colliderCount = 0;

	bool drawColliders;
};