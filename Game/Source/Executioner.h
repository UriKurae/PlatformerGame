#pragma once

#include "Enemy.h"
#include "Entity.h"
#include "DynArray.h"

class Executioner : public Entity
{
public:
	Executioner(iPoint pos);
	virtual ~Executioner() {};

	bool Start() override;

	bool Update(float dt) override;

	void Draw() override;

	bool CleanUp() override;

	void TakeDamage(int damage);

	void Attack();

	void EnemyDies();

	bool FindTarget(Player* player, float dt);

	bool ChaseTarget(float dt);

	bool Patrol(float dt);

	void HandleCollisions(float dt);

	// Load and save functions for each module
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&);



private:

	int speedX;
	int speedY;

	int life;
	int damage;
	int attackSpeed;

	bool isAlive;
	bool savedIsAlive;

	EnemyState currentState;
	int pathCooldown;

	Animation* currentAnim;

	DynArray<iPoint> path;

	Animation idleAnim;
	Animation skillAnim;
	Animation deathAnim;
	Animation hurtAnim;

	iPoint offsetPathfinding;

	int indexPath;


};
