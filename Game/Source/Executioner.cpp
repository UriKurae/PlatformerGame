#include "Executioner.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EnemyManager.h"
#include "Pathfinding.h"
#include "Collisions.h"
#include "Input.h"
#include "Player.h"
#include "Map.h"
#include <math.h>



Executioner::Executioner(iPoint pos) : Enemy(pos)
{
	name.Create("executioner");
	
	// Idle animation
	idleAnim.PushBack({ 15, 421, 68, 93 });
	idleAnim.PushBack({ 104, 420, 68, 93 });
	idleAnim.PushBack({ 191, 420, 68, 91 });
	idleAnim.PushBack({ 6, 523, 68, 90 });
	idleAnim.PushBack({ 95, 522, 68, 90 });
	idleAnim.PushBack({ 182, 522, 68, 87 });
	idleAnim.PushBack({ 275, 524, 68, 87 });
	idleAnim.loop = true;

	// Skill animation
	skillAnim.PushBack({ 433, 398, 67, 93 });
	skillAnim.PushBack({ 529, 392, 72, 99 });
	skillAnim.PushBack({ 623, 395, 45, 97 });
	skillAnim.PushBack({ 710, 395, 45, 97 });
	skillAnim.PushBack({ 772, 396, 47, 96 });
	skillAnim.PushBack({ 836, 395, 50, 96 });
	skillAnim.PushBack({ 423, 503, 69, 96 });
	skillAnim.PushBack({ 500, 507, 116, 96 });
	skillAnim.PushBack({ 625, 505, 76, 96 });
	skillAnim.PushBack({ 713, 504, 50, 96 });
	skillAnim.PushBack({ 784, 506, 57, 97 });
	skillAnim.PushBack({ 854, 500, 72, 102 });
	skillAnim.loop = false;

	// Death animation
	deathAnim.PushBack({ 1135, 16, 79, 94 });
	deathAnim.PushBack({ 1230, 16, 79, 94 });
	deathAnim.PushBack({ 1327, 21, 79, 90 });
	deathAnim.PushBack({ 1418, 29, 48, 81 });
	deathAnim.PushBack({ 1488, 29, 46, 81 });
	deathAnim.PushBack({ 1550, 28, 31, 81 });
	deathAnim.PushBack({ 1597, 30, 30, 66 });
	deathAnim.PushBack({ 1643, 39, 33, 58 });
	deathAnim.PushBack({ 1691, 45, 39, 45 });
	deathAnim.PushBack({ 1752, 45, 39, 45 });
	deathAnim.PushBack({ 1133, 133, 30, 27 });
	deathAnim.PushBack({ 1231, 134, 25, 26 });
	deathAnim.PushBack({ 1329, 137, 23, 21 });
	deathAnim.PushBack({ 1420, 136, 21, 19 });
	deathAnim.PushBack({ 1490, 136, 21, 19 });
	deathAnim.PushBack({ 1552, 135, 21, 19 });
	deathAnim.PushBack({ 1598, 134, 21, 19 });
	deathAnim.PushBack({ 1645, 135, 18, 19 });
	deathAnim.PushBack({ 1665, 135, 18, 19 });
	deathAnim.loop = false;

	//Take damage animation
	hurtAnim.PushBack({ 1115, 413, 68, 89 });
	hurtAnim.PushBack({ 1204, 412, 68, 89 });
	hurtAnim.loop = false;
}

bool Executioner::Start()
{
	texture = app->enemyManager->executionerTexture;
	
	currentAnim = &idleAnim;
	collider = app->collisions->AddCollider({ position.x - 2, position.y + 10, 37, 80 }, Collider::Type::ENEMY); // 10 stands for offset
	
	speedX = 200;
	currentState = EnemyState::PATROL;
	life = 200;

	return true;
}

bool Executioner::Update(float dt)
{

	idleAnim.speed = 4.0f * dt;
	hurtAnim.speed = 15.0f * dt;
	deathAnim.speed = 6.0f * dt;
	skillAnim.speed = 10.0f * dt;


	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		app->pathFinding->ResetPath(iPoint(position.x / 16, position.y / 16));
	}

   
	if ((currentAnim != &idleAnim) && (hurtAnim.HasFinished()) && (this->life > 0))
	{
		currentAnim = &idleAnim;
	}


	currentAnim->Update();
	collider->SetPos(position.x - 2, position.y + 10);

	if (currentState == EnemyState::PATROL)
	{
		position.x += speedX * dt;
		currentAnim = &idleAnim;
		
		if (Patrol(dt))
			currentState = EnemyState::ALERT;
	}
	else if(currentState == EnemyState::ALERT)
	{
		if (FindTarget(app->player, dt))
			currentState = EnemyState::ATTACK;	
	}
	else if (currentState == EnemyState::ATTACK)
	{
		if (ChaseTarget(dt))
		{
			currentState = EnemyState::PATROL;
		}
	}



	if (this->life <= 0)
		this->EnemyDies();


	return true;
}

bool Executioner::CleanUp()
{
	app->enemyManager->DeleteEnemy(this);
	pathExecutioner.Clear();

	return true;
}

void Executioner::TakeDamage(int damage)
{
	life -= damage;

	if ((this->currentAnim != &this->hurtAnim) && (this->life > 0))
	{
		this->hurtAnim.Reset();
		this->currentAnim = &this->hurtAnim;
	}
}

void Executioner::EnemyDies()
{
	if (currentAnim != &deathAnim)
	{
		deathAnim.Reset();
		currentAnim = &deathAnim;
	}

	if (deathAnim.HasFinished())
	{	
		this->CleanUp();
	}
}

void Executioner::Attack()
{
}

void Executioner::Draw()
{
	if (currentAnim != nullptr)
	{
		app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
	}
	//app->pathFinding->DrawPath();
}

bool Executioner::FindTarget(Player* player, float dt)
{

	if (pathCooldown <= 0)
	{
		pathExecutioner.Clear();

		app->pathFinding->ResetPath(iPoint(position.x / app->map->data.tileWidth, position.y / app->map->data.tileHeight));
		app->pathFinding->PropagateAStar(player);

		pathExecutioner = *(app->pathFinding->ComputePath(player->GetPosition().x, player->GetPosition().y));

		indexPath = pathExecutioner.Count() - 1;
		pathCooldown = 50;

		return true;
	}
	else
	{
		pathCooldown -= 5 * dt;

	}

	return false;
}

bool Executioner::ChaseTarget(float dt)
{	
	if (((position.x / app->map->data.tileWidth) == (pathExecutioner[indexPath].x)) && ((position.y / app->map->data.tileHeight) == (pathExecutioner[indexPath].y)))
	{
		if (indexPath > 1)
		{
			indexPath--;
		}
		else
		{
			if (currentAnim != &skillAnim)
			{
				skillAnim.Reset();
				currentAnim = &skillAnim;
			}
			return true;
		}
	}
	else
	{
		if (pathExecutioner[indexPath].y > position.y / app->map->data.tileHeight)
		{
			position.y += 4;
		}

		if (pathExecutioner[indexPath].y < position.y / app->map->data.tileHeight)
		{
			position.y -= 4;
		}

		if (pathExecutioner[indexPath].x > position.x / app->map->data.tileWidth)
		{
			position.x += 4;
		}

		if (pathExecutioner[indexPath].x < position.x / app->map->data.tileWidth)
		{
			position.x -= 4;
		}

	}

	
	return false;
}

bool Executioner::Patrol(float dt)
{
	if (position.x < 80)
	{
		speedX = -speedX;
	}
	if (position.x > 880)
	{
		speedX = -speedX;
	}

	int vec1 = app->player->GetPosition().x - position.x;
	int vec2 = app->player->GetPosition().y - position.y;

	if (sqrt(pow(vec1, 2) + pow(vec2, 2)) < 200)
		return true;


	return false;

}

bool Executioner::Load(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();

	return true;
}

bool Executioner::Save(pugi::xml_node& node)
{
	pugi::xml_node executioner = node.append_child("position");
	executioner.append_attribute("x").set_value(position.x);
	executioner.append_attribute("y").set_value(position.y);

	return true;
}