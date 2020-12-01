#pragma once

#include "Enemy.h"
#include "Collider.h"


class Bird : public Enemy
{
public:

	Bird(iPoint pos);

	~Bird();

	bool Start();

	bool Update(float dt) override;

	void Attack();

	void Draw() override;


private:
	Animation idleAnim;
	Animation skillAnim;
	Animation deathAnim;

	Collider* collider;
};