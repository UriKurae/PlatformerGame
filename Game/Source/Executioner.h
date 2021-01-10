#pragma once

#include "Enemy.h"
#include "DynArray.h"

class Executioner : public Enemy
{
public:
	Executioner(iPoint pos);
	virtual ~Executioner() {};

	bool Start() override;

	bool Update(float dt) override;

	void Draw() override;

	bool CleanUp() override;

	void TakeDamage(int damage) override;

	void EnemyDies() override;

	bool FindTarget(Player* player, float dt) override;

	bool ChaseTarget(float dt) override;

	bool Patrol(float dt, iPoint playerPos) override;

	void HandleCollisions(float dt) override;

	// Load and save functions for each module
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&);


public:

	Animation idleAnim;

private:

	Animation skillAnim;
	Animation deathAnim;
	Animation hurtAnim;

	iPoint offsetPathfinding;

	int indexPath;
};