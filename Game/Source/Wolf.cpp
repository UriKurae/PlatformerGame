#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Wolf.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Input.h"

Wolf::Wolf(iPoint pos) : Enemy(pos)
{
	name.Create("wolf");

	// Idle animation
	idleAnim.PushBack({ 13,124,38,24 });
	idleAnim.PushBack({ 76,127,39,21 });
	idleAnim.PushBack({ 139,127,39,21 });
	idleAnim.PushBack({ 203,127,39,21 });
	idleAnim.PushBack({ 266,127,39,21 });
	idleAnim.PushBack({ 329,127,40,21 });
	idleAnim.PushBack({ 266,127,39,21 });
	idleAnim.PushBack({ 203,127,39,21 });
	idleAnim.PushBack({ 139,127,39,21 });
	idleAnim.PushBack({ 76,127,39,21 });
	idleAnim.PushBack({ 13,124,38,24 });
	idleAnim.loop = true;
	
	// Run animation 
	runAnim.PushBack({ 9,5,45,29 });
	runAnim.PushBack({ 78,12,44,22 });
	runAnim.PushBack({ 144,8,44,26 });
	runAnim.PushBack({ 221,9,43,25 });
	runAnim.PushBack({ 278,8,45,26 });
	runAnim.loop = true;

	// Walk animation
	walkAnim.PushBack({ 9,169,44,24 });
	walkAnim.PushBack({ 74,168,45,25 });
	walkAnim.PushBack({ 138,167,45,26 });
	walkAnim.PushBack({ 202,168,45,25 });
	walkAnim.PushBack({ 266,169,45,24 });
	walkAnim.PushBack({ 330,169,44,24 });
	walkAnim.PushBack({ 393,169,44,24 });
	walkAnim.PushBack({ 457,168,45,25 });
	walkAnim.PushBack({ 522,167,45,26 });
	walkAnim.PushBack({ 586,168,45,25 });
	walkAnim.PushBack({ 649,170,45,24 });
	walkAnim.PushBack({ 713,170,44,24 });
	walkAnim.loop = true;


	// Jump Animation
	jumpAnim.PushBack({13,73,44,26});
	jumpAnim.PushBack({79,65,39,34});
	jumpAnim.PushBack({141,58,46,37});
	jumpAnim.PushBack({207,60,47,24});
	jumpAnim.PushBack({270,56,41,39});
	jumpAnim.PushBack({336,71,39,28});
	jumpAnim.loop = false;

	// Take damage animation
	hurtAnim.PushBack({ 12,207,45,29 });
	hurtAnim.PushBack({ 77,202,39,34 });
	hurtAnim.loop = false;

	// Die animation
	deathAnim.PushBack({ 13, 258, 51,24 });
	deathAnim.loop = false;
}

bool Wolf::Start()
{
	texture = app->enemyManager->wolfTexture;
	collider = app->collisions->AddCollider({ position.x - 2, position.y + 10, 38, 24 }, Collider::Type::ENEMY); // 10 stands for offset
	currentAnim = &idleAnim;
	life = 60;

	return true;
}

bool Wolf::Update(float dt)
{
	idleAnim.speed = 4.50f * dt;
	hurtAnim.speed = 10.0f * dt;
	deathAnim.speed = 1.0f * dt;
	jumpAnim.speed = 4.50f * dt;


	if ((currentAnim != &idleAnim) && (life > 0))
	{
		if ((hurtAnim.IsPlaying() == false))
		{
			idleAnim.Reset();
			currentAnim = &idleAnim;
		}

	}

	if (life == 0)
	{
		if (currentAnim != &deathAnim)
		{
			deathAnim.Reset();
			currentAnim = &deathAnim;
		}
	}

	collider->SetPos(position.x, position.y);
	currentAnim->Update();

	if (life <= 0)
		EnemyDies();

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		FindTarget(app->player);
	}

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
	{
		ChaseTarget();
	}

	return true;
}

bool Wolf::CleanUp()
{
	app->tex->UnLoad(texture);
	app->enemyManager->DeleteEnemy(this);
	
	return true;
}

void Wolf::TakeDamage(int damage)
{
	life -= damage;	

	if ((currentAnim != &hurtAnim) && (life > 0))
	{
		hurtAnim.Reset();
		currentAnim = &hurtAnim;
	}
}

void Wolf::EnemyDies()
{
	if (currentAnim != &deathAnim)
	{
		deathAnim.Reset();
		currentAnim = &deathAnim;
	}

	if (deathAnim.HasFinished())
		CleanUp();
}


bool Wolf::FindTarget(Player* player)
{
	pathWolf.Clear();
	app->pathFinding->ResetPath(iPoint(position.x / 16, position.y / 16));
	app->pathFinding->PropagateAStar(player);

	pathWolf = *(app->pathFinding->ComputePath(player->GetPosition().x, player->GetPosition().y));

	indexPath = pathWolf.Count() - 1;


	return true;
}

bool Wolf::ChaseTarget()
{
	if (((position.x / app->map->data.tileWidth) == (pathWolf[indexPath].x)) && ((position.y / app->map->data.tileHeight) == (pathWolf[indexPath].y)))
	{
		if (indexPath > 0)
		{
			indexPath--;
		}
	}
	else
	{
		if (pathWolf[indexPath].y > position.y / app->map->data.tileHeight)
		{
			position.y += 4;
		}

		if (pathWolf[indexPath].y < position.y / app->map->data.tileHeight)
		{
			position.y -= 20;

			if (currentAnim != &jumpAnim)
			{
				jumpAnim.Reset();
				currentAnim = &jumpAnim;
			}
		}

		if (pathWolf[indexPath].x > position.x / app->map->data.tileWidth)
		{
			position.x += 4;
		}

		if (pathWolf[indexPath].x < position.x / app->map->data.tileWidth)
		{
			position.x -= 4;
		}

	}


	return true;
}

bool Wolf::Load(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();

	return true;
}

bool Wolf::Save(pugi::xml_node& node)
{
	pugi::xml_node wolf = node.append_child("position");
	wolf.append_attribute("x").set_value(position.x);
	wolf.append_attribute("y").set_value(position.y);

	return true;
}

void Wolf::Attack()
{
}

void Wolf::Draw()
{
	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
}
