#pragma once

#include "Enemy.h"
#include "DynArray.h"

class Executioner : public Enemy
{
public:
	Executioner(iPoint pos);
	virtual ~Executioner() {};

	bool Start();

	bool Update(float dt) override;

	void Attack();

	void Draw() override;

	bool CleanUp() override;

	bool FindTarget(Player* player) override;

	bool ChaseTarget() override;

	// Load and save functions for each module
	bool Load(pugi::xml_node&) override;

	bool Save(pugi::xml_node&) override;



private:
	Animation idleAnim;
	Animation skillAnim;
	Animation deathAnim;
	DynArray<iPoint> path;

};
