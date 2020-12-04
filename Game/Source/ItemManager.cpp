#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "ItemManager.h"
#include "GreenGem.h"
#include "Collisions.h"

ItemManager::ItemManager()
{
	name.Create("itemmanager");
}

ItemManager::~ItemManager()
{
	items.Clear();
}

bool ItemManager::Start()
{
	texture = app->tex->Load("Assets/Textures/Coins/coins.png");

	ListItem<Item*>* it = items.start;

	while (it != nullptr)
	{
		if (it->data->active)
			it->data->Start();

		it = it->next;
	}

	return true;
}

bool ItemManager::Update(float dt)
{
	ListItem<Item*>* it = items.start;

	while (it != nullptr)
	{
		if (it->data->active)
			it->data->Update(dt);

		it = it->next;
	}

	return true;
}

bool ItemManager::PostUpdate()
{
	ListItem<Item*>* it = items.start;

	while (it != nullptr)
	{
		if (it->data->active)
			it->data->Draw();

		it = it->next;
	}

	return true;
}

bool ItemManager::CleanUp()
{
	ListItem<Item*>* it = items.start;

	while (it != nullptr)
	{
		if (it->data->active)
			it->data->CleanUp();

		it = it->next;
	}

	return true;
}

Item* ItemManager::AddItem(ItemType type, iPoint pos)
{
	Item* item;
	
	switch (type)
	{
	case ItemType::GEM:
		item = new GreenGem(pos);
		break;
	}

	items.Add(item);
	return item;
}

void ItemManager::DeleteItem(Item* item)
{
	ListItem<Item*>* it = items.start;

	while (it != nullptr)
	{
		if (item == it->data)
		{
			it->data->collider->pendingToDelete = true;
			items.Del(it);

			break;
		}

		it = it->next;
	}
}