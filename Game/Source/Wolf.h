#pragma once

#include "Enemy.h"
#include "DynArray.h"

class Wolf : public Enemy
{
public:
	
	Wolf(iPoint pos);
	
	virtual ~Wolf() {};

	bool Start();

	bool Update(float dt) override;

	void Attack();

	void Draw() override;

	bool CleanUp() override;

	void TakeDamage(int damage) override;

	void EnemyDies() override;
	
	bool FindTarget(Player* player) override;

	bool ChaseTarget() override;

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) override;
	

private:

	Animation hurtAnim;
	Animation idleAnim;
	Animation runAnim;
	Animation walkAnim;
	Animation deathAnim;
	Animation jumpAnim;


	int indexPath;
	DynArray<iPoint> pathWolf;

};