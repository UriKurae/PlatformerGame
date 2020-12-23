#include "App.h"
#include "SceneManager.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Player.h"
#include "Executioner.h"
#include "Wolf.h"
#include "GreenGem.h"
#include "RedHeart.h"

EntityManager::EntityManager()
{
}

bool EntityManager::Start()
{
	// Loading all textures for each enemy
	executionerTexture = app->tex->Load("Assets/Textures/Enemies/Executioner/executioner.png");
	wolfTexture = app->tex->Load("Assets/Textures/Enemies/wolf/wolf.png");

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
			entities.Del(item);
			break;
		}
		item = item->next;
	}
}

bool EntityManager::Update(float dt)
{
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

	return true;
}

bool EntityManager::CleanUp()
{
	ListItem<Entity*>* item = entities.start;

	while ((item != nullptr))
	{
		item->data->CleanUp();
		item = item->next;
	}

	return true;
}
