#pragma once

#include "Enemy.h"
#include "DynArray.h"
#include "SString.h"


class Wolf : public Enemy
{
public:
	
	Wolf(iPoint pos);

	virtual ~Wolf() {};
	
	bool Start() override;

	bool Update(float dt) override;

	void Attack();

	void Draw() override;

	bool CleanUp() override;

	void TakeDamage(int damage) override;

	void EnemyDies() override;
	
	bool FindTarget(Player* player, float dt) override;

	bool ChaseTarget(float dt) override;

	bool Patrol(float dt) override;

	void HandleCollisions(float dt) override;

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) override;
	

private:

	SString direction;

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

	bool blockFall;

};