#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"

Enemy::Enemy(iPoint pos)
{
	position = pos;
}

Enemy::~Enemy()
{
}

bool Enemy::Update(float dt)
{
	if (currentAnim != nullptr)
		currentAnim->Update();
	
	return true;
}

void Enemy::Draw()
{
	if (currentAnim != nullptr)
	{
		app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
	}
}

bool Enemy::FindTarget(Player* player)
{
	return true;
}

bool Enemy::ChaseTarget()
{
	return true;
}

void Enemy::TakeDamage(int damage)
{
}

void Enemy::EnemyDies()
{
}

bool Enemy::Patrol(float dt)
{
	return true;
}

bool Enemy::CleanUp()
{
	return true;
}

