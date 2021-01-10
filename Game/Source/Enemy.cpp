#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Enemy.h"

Enemy::Enemy(iPoint pos) : Entity(pos)
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	return true;
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

bool Enemy::FindTarget(Player* player, float dt)
{
	return true;
}

bool Enemy::ChaseTarget(float dt)
{
	return true;
}

void Enemy::TakeDamage(int damage)
{
}

void Enemy::EnemyDies()
{
}

bool Enemy::Patrol(float dt, iPoint playerPos)
{
	return true;
}

bool Enemy::Save(pugi::xml_node&)
{
	return true;
}

bool Enemy::Load(pugi::xml_node&)
{
	return true;
}

void Enemy::HandleCollisions(float dt)
{

}

bool Enemy::CleanUp()
{
	return true;
}

