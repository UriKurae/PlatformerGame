#include "App.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Executioner.h"

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
		case EntityType::EXECUTIONER: ret = new Executioner(pos);
	}
	if (ret != nullptr)
		entities.Add(ret);

	return ret;
}

void EntityManager::DeleteEntity(Entity* entity)
{

	ListItem <Entity*> *item = entities.start;

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
	ListItem<Entity*>* item = entities.start;

	while ((item != nullptr))
	{
		item->data->Update(dt);
		item = item->next;
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
