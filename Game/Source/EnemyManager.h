#pragma once

#include "Module.h"
#include "Enemy.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define MAX_ENEMIES 100

class EnemyManager : public Module
{
public:
	EnemyManager();
	virtual ~EnemyManager();

	bool Update(float dt);

	void Draw();

	Enemy* AddEnemy(EnemyType type, iPoint pos);
	void DeleteEnemy(Enemy* enemy);


	bool Save(pugi::xml_node&) override;
	bool Load(pugi::xml_node&) override;

public:
	List<Enemy*> enemies;

};
