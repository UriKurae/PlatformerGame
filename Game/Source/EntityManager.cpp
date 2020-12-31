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
		if (item->data->type == EntityType::PLAYER)
		{
			item->data->Save(node.append_child("player"));
			break;
		}

		item = item->next;
	}

	

	ListItem<Entity*>* items = entities.start;
	pugi::xml_node itemsNode = node.append_child("items");

	int numGems = 0;
	int numHearts = 0;

	while (items != nullptr)
	{
		if (items->data->type == EntityType::GEM || items->data->type == EntityType::HEART)
		{
			items->data->Save(itemsNode.append_child(items->data->name.GetString()));

			if (items->data->type == EntityType::GEM)
				numGems += 1;
			else if (items->data->type == EntityType::HEART)
				numHearts += 1;
		}

		items = items->next;
	}

	pugi::xml_node numItems = node.append_child("num_items");
	numItems.append_child("gems").append_attribute("value").set_value(numGems);
	numItems.append_child("hearts").append_attribute("value").set_value(numHearts);

	ListItem<Enemy*>* it = enemies.start;
	pugi::xml_node enemiesNode = node.append_child("enemies");

	int numExecutioners = 0;
	int numWolves = 0;
	while (it != nullptr)
	{
		it->data->Save(enemiesNode.append_child(it->data->name.GetString()));

		if (it->data->type == EntityType::EXECUTIONER)
			numExecutioners += 1;
		else if (it->data->type == EntityType::WOLF)
			numWolves += 1;

		it = it->next;
	}

	pugi::xml_node numEnemies = node.append_child("num_enemies");
	numEnemies.append_child("executioners").append_attribute("value").set_value(numExecutioners);
	numEnemies.append_child("wolves").append_attribute("value").set_value(numWolves);
	return true;
}

bool EntityManager::Load(pugi::xml_node& node)
{
	/*CleanUp();
	Start();

	int numWolves = 0;
	int numExecutioners = 0;

	numExecutioners = node.child("num_enemies").child("executioners").attribute("value").as_int();
	numWolves = node.child("num_enemies").child("wolves").attribute("value").as_int();

	CreateEntity(EntityType::PLAYER, iPoint(0, 0));

	while (numExecutioners > 0)
	{
		CreateEntity(EntityType::EXECUTIONER, iPoint(0, 0));
		numExecutioners -= 1;
	}

	while (numWolves > 0)
	{
		CreateEntity(EntityType::WOLF, iPoint(0, 0));
		numWolves -= 1;
	}

	ListItem<Entity*>* item = entities.start;
	pugi::xml_node executioner = node.child("enemies").child("executioner");
	pugi::xml_node wolf = node.child("enemies").child("wolf");

	while (item != nullptr)
	{
		if (item->data->name == "player")
		{
			item->data->Load(node.child("player"));
		}
		else if (item->data->name == "executioner")
		{
			item->data->Load(executioner);
			executioner = executioner.next_sibling("executioner");
		}
		else if (item->data->name == "wolf")
		{
			item->data->Load(wolf);
			wolf = wolf.next_sibling("wolf");
		}

		item = item->next;
	}*/

	return true;
}

bool EntityManager::Start()
{
	// Loading all textures for each enemy
	executionerTexture = app->tex->Load("Assets/Textures/Enemies/Executioner/executioner.png");
	wolfTexture = app->tex->Load("Assets/Textures/Enemies/wolf/wolf.png");
	collectiblesTexture = app->tex->Load("Assets/Textures/Collectibles/collectibles.png");

	drawColliders = false;

	return true;
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
			int index = enemies.Find((Enemy*)item->data);
			RemoveCollider(item->data->collider);
			enemies.Del(enemies.At(index));
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

void EntityManager::DeleteEntities()
{
	ListItem<Entity*>* item = entities.start;
	while (item != nullptr)
	{
		item->data->CleanUp();
		item = item->next;
	}
	ClearLists();
}

void EntityManager::RemoveCollider(Collider* c)
{
	ListItem<Collider*>* item = colliders.start;
	while (item != nullptr)
	{
		if (c == item->data)
		{
			colliders.Del(item);
			break;
		}
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
