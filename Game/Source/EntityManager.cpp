#include "App.h"
#include "Textures.h"
#include "Player.h"
#include "Wolf.h"
#include "Executioner.h"
#include "GreenGem.h"
#include "RedHeart.h"
#include "EntityManager.h"
#include "SceneManager.h"

EntityManager::EntityManager()
{
	name.Create("entitymanager");
}

bool EntityManager::Save(pugi::xml_node& node)
{
	ListItem<Entity*>* item = entities.start;
	while (item != nullptr)
	{
		pugi::xml_node tmp = node.append_child(item->data->name.GetString());
		item->data->Save(tmp);

		item = item->next;
	}

	return true;
}

bool EntityManager::Load(pugi::xml_node& node)
{
	ListItem<Entity*>* item = entities.start;
	while (item != nullptr)
	{
		item->data->Load(node.child(item->data->name.GetString()));

		item = item->next;
	}

	return true;
}

bool EntityManager::Start()
{
	// Loading all textures for each enemy
	executionerTexture = app->tex->Load("Assets/Textures/Enemies/Executioner/executioner.png");
	wolfTexture = app->tex->Load("Assets/Textures/Enemies/wolf/wolf.png");
	collectiblesTexture = app->tex->Load("Assets/Textures/Collectibles/collectibles.png");

	drawColliders = false;

	// You must start the entity when it's created

	/*ListItem<Entity*>* item = entities.start;
	while (item != nullptr)
	{
		item->data->Start();
		item = item->next;
	}*/

	return true;
}

bool EntityManager::Update(float dt)
{
	//DeleteResidualColliders();

	if (app->sceneManager->isPaused == false)
	{
		ListItem<Entity*>* item = entities.start;

		while ((item != nullptr))
		{
			item->data->Update(dt);
			item = item->next;
		}
	}

	return true;
}

bool EntityManager::Draw()
{
	ListItem<Entity*>* item = entities.start;

	while ((item != nullptr))
	{
		item->data->Draw();
		item = item->next;
	}

	if(drawColliders)
		DrawColliders();

	return true;
}

bool EntityManager::CleanUp()
{
	app->tex->UnLoad(executionerTexture);
	app->tex->UnLoad(wolfTexture);
	app->tex->UnLoad(collectiblesTexture);

	ListItem<Entity*>* item = entities.start;
	while ((item != nullptr))
	{
		item->data->CleanUp();
		item = item->next;
	}

	ClearLists();

	return true;
}

bool EntityManager::DeleteResidualColliders()
{
	// Delete colliders scheduled for deletion
	/*for (uint i = 0; i < colliders.Count(); ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}*/

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, iPoint pos)
{
	Entity* ret = nullptr;

	switch (type)
	{
		case EntityType::EXECUTIONER: ret = new Executioner(pos); break;
		case EntityType::WOLF: ret = new Wolf(pos); break;
		case EntityType::GEM: ret = new GreenGem(pos); break;
		case EntityType::HEART: ret = new RedHeart(pos); break;
		case EntityType::PLAYER: ret = new Player(pos); break;
		default: break;
	}

	if (type == EntityType::EXECUTIONER || type == EntityType::WOLF)
		enemies.Add((Enemy*)ret);

	else if (type == EntityType::GEM || type == EntityType::HEART)
		items.Add((Item*)ret);

	if (ret != nullptr)
		entities.Add(ret);

	return ret;
}

void EntityManager::DeleteEntity(Entity* entity)
{
	ListItem <Entity*>* item = entities.start;

	while (item != nullptr)
	{
		if (item->data == entity)
		{
			RemoveCollider(item->data->collider);
			entities.Del(item);
			break;
		}
		item = item->next;
	}
}

void EntityManager::DeleteColliders()
{
	ListItem<Entity*>* item = entities.start;

	while (item != nullptr)
	{
		if (item->data->collider != nullptr)
		{
			delete item->data->collider;
			item->data->collider = nullptr;
		}
		item = item->next;
	}
}

void EntityManager::RemoveCollider(Collider* c)
{
	ListItem<Collider*>* item = colliders.start;
	while (item != nullptr)
	{
		if (c == item->data)
			colliders.Del(item);

		item = item->next;
	}

}

void EntityManager::DrawColliders()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < colliders.Count(); ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case Collider::Type::NONE: // White
			app->render->DrawRectangle(colliders[i]->rect, { 255, 255, 255, alpha });
			break;

		case Collider::Type::PLAYER: // Green
			app->render->DrawRectangle(colliders[i]->rect, { 0, 255, 0, alpha });
			break;

		case Collider::Type::ENEMY: // Red
			app->render->DrawRectangle(colliders[i]->rect, { 255, 0, 0, alpha });
			break;

		case Collider::Type::PLAYER_HIT: // Blue
			app->render->DrawRectangle(colliders[i]->rect, { 0, 0, 255, alpha });
			break;

		case Collider::Type::ITEM: // Purple
			app->render->DrawRectangle(colliders[i]->rect, { 255, 0, 255, alpha });
			break;
		}
	}
}

void EntityManager::ClearLists()
{
	entities.Clear();
	enemies.Clear();
	items.Clear();
	colliders.Clear();
}

Collider* EntityManager::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	ret = new Collider(rect, type);
	colliders.Add(ret);

	return ret;
}
