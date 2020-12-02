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

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

private:
	Animation idleAnim;
	Animation walkAnim;
	Animation takeDamageAnim;
	Animation dieAnim;

	Collider* collider;
};