#include "App.h"
#include "EnemyManager.h"
#include "Executioner.h"
#include "Wolf.h"
#include "Ghost.h"


EnemyManager::EnemyManager(): Module()
{
	name.Create("enemymanager");
}

EnemyManager::~EnemyManager()
{
	enemies.Clear();
}

bool EnemyManager::Update(float dt)
{
	ListItem<Enemy*>* item = enemies.start;

	while (item != nullptr)
	{
		item->data->Update(dt);
		item = item->next;
	}

	Draw();

	return true;
}

void EnemyManager::Draw()
{
	ListItem<Enemy*>* item = enemies.start;

	while (item != nullptr)
	{
		item->data->Draw();
		item = item->next;
	}
}

Enemy* EnemyManager::AddEnemy(EnemyType type, iPoint pos)
{
	Enemy* enemy;
	switch (type)
	{
	case EnemyType::EXECUTIONER:
		enemy = new Executioner(pos);
		break;	
	case EnemyType::BIRD:
		enemy = new Ghost(pos);
		break;
	case EnemyType::GROUND:
		enemy = new Wolf(pos);
		break;	
	}

	enemies.Add(enemy);
	
	return enemy;
}

void EnemyManager::DeleteEnemy(Enemy* enemy)
{
	ListItem<Enemy*>* item = enemies.start;
	
	while (item != nullptr)
	{
		if (enemy == item->data)
		{
			item->data->collider->pendingToDelete = true;
			enemies.Del(item);
			break;
		}

		item = item->next;
	}
}

bool EnemyManager::Save(pugi::xml_node& node)
{
	ListItem<Enemy*>* item = enemies.start;
	pugi::xml_node enemies = node.append_child("enemies");
	while (item != nullptr)
	{
		item->data->Save(enemies.append_child(item->data->name.GetString()));
		item = item->next;
	}

	return true;
}

bool EnemyManager::Load(pugi::xml_node& node)
{
	ListItem<Enemy*>* item = enemies.start;
	pugi::xml_node enemies = node.child("enemies");
	while (item != nullptr)
	{
		item->data->Load(enemies.child(item->data->name.GetString()));
		item = item->next;
	}

	return true;
}
