#include "App.h"
#include "SceneManager.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Player.h"
#include "Map.h"
#include "Executioner.h"
#include "EntityManager.h"
#include "EnemyManager.h"
#include "Pathfinding.h"

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

	Start();
}

bool Executioner::Start()
{
	texture = app->entityManager->executionerTexture;
	
	isAlive = true;

	path.Clear();
	collider = app->entityManager->AddCollider({ position.x - 2, position.y + 10, 37, 80 }, Collider::Type::ENEMY); // 10 stands for offset
	currentAnim = &idleAnim;

	offsetPathfinding.x = 21;
	offsetPathfinding.y = 47;

	speedX = 100;
	life = 200;
	currentState = EnemyState::PATROL;
	type = EntityType::EXECUTIONER;

	return true;
}

bool Executioner::Update(float dt)
{
	idleAnim.speed = 4.0f * dt;
	hurtAnim.speed = 15.0f * dt;
	deathAnim.speed = 6.0f * dt;
	skillAnim.speed = 10.0f * dt;

	if ((currentAnim != &idleAnim) && (hurtAnim.HasFinished()) && (this->life > 0))
		currentAnim = &idleAnim;

	/*if (player->loadedGame)
		isAlive = savedIsAlive;*/


	if(currentAnim != nullptr)
		currentAnim->Update();

	if(collider != nullptr)
		collider->SetPos(position.x - 2, position.y + 10);

	//if(dt < 0.5)
	HandleCollisions(dt);

	if (this->life <= 0)
	{
		this->isAlive = false;
		EnemyDies();
	}

	return true;
}

bool Executioner::CleanUp()
{
	isAlive = false;
	//collider->pendingToDelete = true;
	app->entityManager->DeleteEntity(this);
	path.Clear();

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
		currentAnim = &deathAnim;
	}

	if (deathAnim.HasFinished())
	{	
		app->sceneManager->executionersKilled += 1;
		this->CleanUp();
		deathAnim.Reset();
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
	
	if (app->map->viewHitboxes == true)
	{
		app->pathFinding->DrawPath(path);
	}
	
}

bool Executioner::FindTarget(Player* player, float dt)
{
	if (pathCooldown <= 0)
	{
		path.Clear();

		app->pathFinding->ResetPath(iPoint(position.x / app->map->data.tileWidth, position.y / app->map->data.tileHeight));
		app->pathFinding->PropagateAStar(player);

		path = *(app->pathFinding->ComputePath(player->GetPosition().x, player->GetPosition().y));

		indexPath = path.Count() - 1;
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
	if (path.Count() > 0)
	{
		if ((((position.x + offsetPathfinding.x) / app->map->data.tileWidth) == (path[indexPath].x)) && (((position.y + offsetPathfinding.y) / app->map->data.tileHeight) == (path[indexPath].y)))
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
			if (path[indexPath].y > ((position.y + offsetPathfinding.y) / app->map->data.tileHeight))
				position.y += 100 * dt;

			if (path[indexPath].y < ((position.y + offsetPathfinding.y) / app->map->data.tileHeight))
				position.y -= 100 * dt;

			if (path[indexPath].x > ((position.x + offsetPathfinding.x) / app->map->data.tileWidth))
				position.x += 100 * dt;

			if (path[indexPath].x < ((position.x + offsetPathfinding.x) / app->map->data.tileWidth))
				position.x -= 100 * dt;
		}
	}
	
	return false;
}

bool Executioner::Patrol(float dt, iPoint playerPos)
{
	position.x += speedX * dt;

	int vec1 = playerPos.x - position.x;
	int vec2 = playerPos.y - position.y;

	if (sqrt(pow(vec1, 2) + pow(vec2, 2)) < 200)
		return true;

	return false;
}

bool Executioner::Load(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();
	currentState = (EnemyState)node.child("current_state").attribute("value").as_int();

	return true;
}

bool Executioner::Save(pugi::xml_node& node)
{
	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x").set_value(position.x);
	pos.append_attribute("y").set_value(position.y);
	
	pugi::xml_node state = node.append_child("current_state");
	state.append_attribute("value").set_value((int)currentState);

	pugi::xml_node alive = node.append_child("is_alive");
	alive.append_attribute("value").set_value(isAlive);

	return true;
}

void Executioner::HandleCollisions(float dt)
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint executionerPosTop = app->map->WorldToMap(position.x + 2, position.y - 3);
	iPoint executionerPosBottom = app->map->WorldToMap(position.x + 15, position.y + 80);

	iPoint executionerPosRight = app->map->WorldToMap(position.x + 30 + 10, position.y + 40);
	iPoint executionerPosLeft = app->map->WorldToMap(position.x - 10, position.y + 40);

	iPoint executionerPosTopRight = app->map->WorldToMap(position.x + 30, position.y - 3);
	iPoint executionerPosTopLeft = app->map->WorldToMap(position.x - 3, position.y - 3);

	iPoint executionerPosBottomRight = app->map->WorldToMap(position.x + 30, position.y + 80);
	iPoint executionerPosBottomLeft = app->map->WorldToMap(position.x - 3, position.y + 80);


	while (layer != NULL)
	{
		if (layer->data->name == "HitBoxes")
		{
			// Here we get the surrounders player's tiles
			uint playerIdBottom = layer->data->Get(executionerPosBottom.x, executionerPosBottom.y);

			uint playerIdRight = layer->data->Get(executionerPosRight.x, executionerPosRight.y);
			uint playerIdLeft = layer->data->Get(executionerPosLeft.x, executionerPosLeft.y);

			uint playerIdBottomRight = layer->data->Get(executionerPosBottomRight.x, executionerPosBottomRight.y);
			uint playerIdBottomLeft = layer->data->Get(executionerPosBottomLeft.x, executionerPosBottomLeft.y);

			if (playerIdRight == 1163)
			{
				position.x -= 100 * dt;
				speedX = -speedX;
				currentState = EnemyState::PATROL;
			}

			if (playerIdLeft == 1163)
			{
				position.x += 100 * dt;
				speedX = -speedX;
				currentState = EnemyState::PATROL;
			}
			break;
		}

		layer = layer->next;
	}
}
