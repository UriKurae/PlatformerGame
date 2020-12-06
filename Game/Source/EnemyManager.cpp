#include "App.h"
#include "Textures.h"
#include "EnemyManager.h"
#include "Executioner.h"
#include "Wolf.h"
#include "Ghost.h"
#include "Collisions.h"


EnemyManager::EnemyManager(): Module()
{
	name.Create("enemymanager");
}

EnemyManager::~EnemyManager()
{
	enemies.Clear();
}

bool EnemyManager::Start()
{
	// Loading all textures for each enemy
	executionerTexture = app->tex->Load("Assets/Textures/Enemies/Executioner/executioner.png");
	wolfTexture = app->tex->Load("Assets/Textures/Enemies/wolf/wolf.png");

	return true;
}

bool EnemyManager::Update(float dt)
{
	ListItem<Enemy*>* item = enemies.start;

	while (item != nullptr)
	{
		item->data->Update(dt);
		item = item->next;
	}

	return true;
}

void EnemyManager::Draw()
{
	ListItem<Enemy*>* item = enemies.start;

	while (item != nullptr)
	{
		if(item->data->isAlive)
			item->data->Draw();

		item = item->next;
	}
}

bool EnemyManager::CleanUp()
{
	ListItem<Enemy*>* item = enemies.start;

	while (item != nullptr)
	{
		item->data->CleanUp();
		item = item->next;
	}


	return true;
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
	case EnemyType::WOLF:
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

void EnemyManager::DeleteColliders()
{
	ListItem<Enemy*>* item = enemies.start;

	while (item != nullptr)
	{
		item->data->collider->pendingToDelete = true;
		item = item->next;
	}


}

bool EnemyManager::Save(pugi::xml_node& node)
{
	//ListItem<Enemy*>* item = enemies.start;
	//pugi::xml_node enemies = node.append_child("enemies");
	//while (item != nullptr)
	//{
	//	item->data->Save(enemies.append_child(item->data->name.GetString()));
	//	item = item->next;
	//}

	//return true;
	return true;
}

bool EnemyManager::Load(pugi::xml_node& node)
{
	/*ListItem<Enemy*>* item = enemies.start;
	pugi::xml_node enemies = node.child("enemies");
	while (item != nullptr)
	{
		item->data->Load(enemies.child(item->data->name.GetString()));
		item = item->next;
	}*/

	return true;
}
