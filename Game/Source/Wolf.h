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
	
	bool FindTarget(Player* player, float dt) override;

	bool ChaseTarget() override;

	void PushBacks() override;

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) override;
	

private:

	Animation hurtLeftAnim;
	Animation idleLeftAnim;
	Animation runLeftAnim;
	Animation walkLeftAnim;
	Animation deathLeftAnim;
	Animation jumpLeftAnim;

	Animation hurtRightAnim;
	Animation idleRightAnim;
	Animation runRightAnim;
	Animation walkRightAnim;
	Animation deathRightAnim;
	Animation jumpRightAnim;


	int indexPath;
	DynArray<iPoint> pathWolf;

};