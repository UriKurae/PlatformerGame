#pragma once

#include "Module.h"
#include "Enemy.h"
#include "List.h"

#define MAX_ENEMIES 100

class EnemyManager : Module
{
public:
	EnemyManager();
	virtual ~EnemyManager();

	bool Update(float dt);

	void Draw();

	Enemy* AddEnemy(EnemyType type, iPoint pos);
	void DeleteEnemy(Enemy* enemy);

private:
	List<Enemy*> enemies;
	
};
