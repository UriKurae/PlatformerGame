#pragma once

#include "Enemy.h"
#include "Collider.h"

class Executioner : public Enemy
{
public:
	Executioner(iPoint pos);
	virtual ~Executioner() {};

	bool Start();

	bool Update(float dt) override;

	void Attack();

	void Draw() override;

public:
	Collider* collider;

private:
	Animation idleAnim;
	Animation skillAnim;
	Animation deathAnim;

};
