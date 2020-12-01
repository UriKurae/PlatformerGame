#pragma once

#include "Enemy.h"
#include "Collider.h"


class Ghost : public Enemy
{
public:

	Ghost(iPoint pos);

	~Ghost();

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