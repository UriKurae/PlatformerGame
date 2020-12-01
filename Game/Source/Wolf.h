#pragma once

#include "Enemy.h"

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
	//bool FindTarget(Player* player) override;

	//bool ChaseTarget(iPoint position) override;

private:

	Animation idleAnim;
	Animation runAnim;
	Animation walkAnim;
	Animation deathAnim;

};