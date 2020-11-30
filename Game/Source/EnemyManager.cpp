#include "EnemyManager.h"

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
	Enemy* enemy = new Enemy(pos);

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
			enemies.Del(item);
			break;
		}

		item = item->next;
	}
}
