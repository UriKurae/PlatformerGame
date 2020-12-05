#pragma once

#include "Module.h"
#include "Enemy.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define MAX_ENEMIES 100

struct SDL_Texture;

class EnemyManager : public Module
{
public:
	EnemyManager();
	virtual ~EnemyManager();

	bool Start() override;

	bool Update(float dt) override;

	void Draw();

	Enemy* AddEnemy(EnemyType type, iPoint pos);
	void DeleteEnemy(Enemy* enemy);
	void DeleteColliders();


	bool Save(pugi::xml_node&) override;
	bool Load(pugi::xml_node&) override;

public:
	List<Enemy*> enemies;

	SDL_Texture* executionerTexture;
	SDL_Texture* wolfTexture;

};
