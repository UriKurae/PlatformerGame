#pragma once

#include "Enemy.h"

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


private:
	Animation idleAnim;
	Animation skillAnim;
	Animation deathAnim;

};
