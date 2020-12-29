#include "App.h"
#include "SceneManager.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "Player.h"
#include "Wolf.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Log.h"

Wolf::Wolf(iPoint pos) : Enemy(pos)
{
	name.Create("wolf");

	// Left Animations
	// Idle animation
	idleLeftAnim.PushBack({ 13,124,38,24 });
	idleLeftAnim.PushBack({ 76,127,39,21 });
	idleLeftAnim.PushBack({ 139,127,39,21 });
	idleLeftAnim.PushBack({ 203,127,39,21 });
	idleLeftAnim.PushBack({ 266,127,39,21 });
	idleLeftAnim.PushBack({ 329,127,40,21 });
	idleLeftAnim.PushBack({ 266,127,39,21 });
	idleLeftAnim.PushBack({ 203,127,39,21 });
	idleLeftAnim.PushBack({ 139,127,39,21 });
	idleLeftAnim.PushBack({ 76,127,39,21 });
	idleLeftAnim.PushBack({ 13,124,38,24 });
	idleLeftAnim.loop = true;

	// Run animation 
	runLeftAnim.PushBack({ 9,5,45,29 });
	runLeftAnim.PushBack({ 78,12,44,22 });
	runLeftAnim.PushBack({ 144,8,44,26 });
	runLeftAnim.PushBack({ 207,9,47,25 });
	runLeftAnim.PushBack({ 278,8,45,26 });
	runLeftAnim.loop = true;

	// Walk animation
	walkLeftAnim.PushBack({ 9,169,44,24 });
	walkLeftAnim.PushBack({ 74,168,45,25 });
	walkLeftAnim.PushBack({ 138,167,45,26 });
	walkLeftAnim.PushBack({ 202,168,45,25 });
	walkLeftAnim.PushBack({ 266,169,45,24 });
	walkLeftAnim.PushBack({ 330,169,44,24 });
	walkLeftAnim.PushBack({ 393,169,44,24 });
	walkLeftAnim.PushBack({ 457,168,45,25 });
	walkLeftAnim.PushBack({ 522,167,45,26 });
	walkLeftAnim.PushBack({ 586,168,45,25 });
	walkLeftAnim.PushBack({ 649,170,45,24 });
	walkLeftAnim.PushBack({ 713,170,44,24 });
	walkLeftAnim.loop = true;


	// Jump Animation
	jumpLeftAnim.PushBack({ 13,73,44,26 });
	jumpLeftAnim.PushBack({ 79,65,39,34 });
	jumpLeftAnim.PushBack({ 141,58,46,37 });
	jumpLeftAnim.PushBack({ 207,60,47,24 });
	jumpLeftAnim.PushBack({ 270,56,41,39 });
	jumpLeftAnim.PushBack({ 336,71,39,28 });
	jumpLeftAnim.loop = false;

	// Take damage animation
	hurtLeftAnim.PushBack({ 12,207,45,29 });
	hurtLeftAnim.PushBack({ 77,202,39,34 });
	hurtLeftAnim.loop = false;

	// Die animation
	deathLeftAnim.PushBack({ 13, 258, 51,24 });
	deathLeftAnim.loop = false;

	// Right Animations============================================

	// Idle animation
	idleRightAnim.PushBack({ 716,125,38,24 });
	idleRightAnim.PushBack({ 651,129,39,21 });
	idleRightAnim.PushBack({ 589,128,39,21 });
	idleRightAnim.PushBack({ 525,128,39,21 });
	idleRightAnim.PushBack({ 461,129,39,21 });
	idleRightAnim.PushBack({ 397,128,40,21 });
	idleRightAnim.PushBack({ 461,129,39,21 });
	idleRightAnim.PushBack({ 525,128,39,21 });
	idleRightAnim.PushBack({ 589,128,39,21 });
	idleRightAnim.PushBack({ 651,129,39,21 });
	idleRightAnim.PushBack({ 716,125,38,24 });
	idleRightAnim.loop = true;

	// Run animation 
	runRightAnim.PushBack({ 712,7,45,29 });
	runRightAnim.PushBack({ 646,13,44,22 });
	runRightAnim.PushBack({ 579,10,44,26 });
	runRightAnim.PushBack({ 512,10,43,25 });
	runRightAnim.PushBack({ 445,9,45,26 });
	runRightAnim.loop = true;

	// Walk animation
	walkRightAnim.PushBack({ 714,296,44,24 });
	walkRightAnim.PushBack({ 648,294,45,25 });
	walkRightAnim.PushBack({ 584,293,45,26 });
	walkRightAnim.PushBack({ 520,294,45,25 });
	walkRightAnim.PushBack({ 456,295,45,24 });
	walkRightAnim.PushBack({ 393,295,44,24 });
	walkRightAnim.PushBack({ 329,295,44,24 });
	walkRightAnim.PushBack({ 264,294,45,25 });
	walkRightAnim.PushBack({ 200,293,45,26 });
	walkRightAnim.PushBack({ 137,294,45,25 });
	walkRightAnim.PushBack({ 72,295,45,24 });
	walkRightAnim.PushBack({ 9,295,44,24 });
	walkRightAnim.loop = true;

	// Jump Animation
	jumpRightAnim.PushBack({ 710,74,44,26 });
	jumpRightAnim.PushBack({ 650,66,39,34 });
	jumpRightAnim.PushBack({ 580,59,46,37 });
	jumpRightAnim.PushBack({ 514,61,47,24 });
	jumpRightAnim.PushBack({ 456,57,41,39 });
	jumpRightAnim.PushBack({ 392,72,39,28 });
	jumpRightAnim.loop = false;

	// Take damage animation
	hurtRightAnim.PushBack({ 713,206,45,29 });
	hurtRightAnim.PushBack({ 653,202,39,34 });
	hurtRightAnim.loop = false;

	// Die animation
	deathRightAnim.PushBack({ 700,257,51,24 });
	deathRightAnim.loop = false;

	Start();
}

bool Wolf::Start()
{
	texture = app->entityManager->wolfTexture;
	collider = app->entityManager->AddCollider({ position.x - 2, position.y + 10, 38, 24 }, Collider::Type::ENEMY); // 10 stands for offset
	currentAnim = &walkRightAnim;

	path.Clear();

	isAlive = true;

	life = 60;
	speedX = 100;
	speedY = 10;

	blockFall = false;

	currentState = EnemyState::PATROL;
	type = EntityType::WOLF;

	return true;
}

bool Wolf::Update(float dt)
{
	idleLeftAnim.speed = 4.50f * dt;
	hurtLeftAnim.speed = 10.0f * dt;
	deathLeftAnim.speed = 1.0f * dt;
	runLeftAnim.speed = 8.0f * dt;
	jumpLeftAnim.speed = 4.50f * dt;
	walkLeftAnim.speed = 10.0f * dt;
	
	idleRightAnim.speed = 4.50f * dt;
	hurtRightAnim.speed = 10.0f * dt;
	deathRightAnim.speed = 1.0f * dt;
	runRightAnim.speed = 8.0f * dt;
	jumpRightAnim.speed = 4.50f * dt;
	walkRightAnim.speed = 10.0f * dt;

	if ((currentAnim != &idleLeftAnim) && (this->life > 0))
	{
		if ((hurtLeftAnim.IsPlaying() == false))
		{
			idleLeftAnim.Reset();
			currentAnim = &idleLeftAnim;
		}
	}

	if (this->life == 0)
	{
		if (currentAnim != &deathLeftAnim)
		{
			deathLeftAnim.Reset();
			currentAnim = &deathLeftAnim;
		}
	}

	HandleCollisions(dt);

	if (this->life <= 0)
		EnemyDies();

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	currentAnim->Update();

	return true;
}

bool Wolf::CleanUp()
{
	this->isAlive = false;
	app->entityManager->DeleteEntity(this);
	path.Clear();

	return true;
}

void Wolf::TakeDamage(int damage)
{
	life -= damage;	

	if ((currentAnim != &hurtLeftAnim) && (life > 0))
	{
		hurtLeftAnim.Reset();
		currentAnim = &hurtLeftAnim;
	}
}

void Wolf::EnemyDies()
{
	if (currentAnim != &deathLeftAnim)
	{
		deathLeftAnim.Reset();
		currentAnim = &deathLeftAnim;
	}

	if (deathLeftAnim.HasFinished())
	{
		app->sceneManager->wolvesKilled += 1;
		this->CleanUp();
	}
}

bool Wolf::Patrol(float dt, iPoint playerPos)
{
	position.x += speedX * dt;
	
	int vec1 = playerPos.x - position.x;
	int vec2 = playerPos.y - position.y;

	if (sqrt(pow(vec1, 2) + pow(vec2, 2)) < 150)
		return true;

	return false;
}

bool Wolf::FindTarget(Player* player, float dt)
{
	path.Clear();
	app->pathFinding->ResetPath(iPoint(position.x / app->map->data.tileWidth, position.y / app->map->data.tileHeight));
	app->pathFinding->PropagateAStar(player);

	path = *(app->pathFinding->ComputePath(player->GetPosition().x, player->GetPosition().y));

	indexPath = path.Count() - 1;
	
	return true;
}

bool Wolf::ChaseTarget(float dt)
{
	if (path.Count() > 0)
	{
		if ((position.x / app->map->data.tileWidth) == (path[indexPath].x))
		{
			if (indexPath > 1)
				indexPath--;
			else
				return true;
		}
		else
		{
			if (path[indexPath].y > position.y / app->map->data.tileHeight)
			{
				if (blockFall == false)
					position.y += 4;
			}

			if (path[indexPath].y < position.y / app->map->data.tileHeight)
			{
				if (currentAnim != &jumpLeftAnim)
				{
					jumpLeftAnim.Reset();
					currentAnim = &jumpLeftAnim;
				}

				if (blockFall == false)
					position.y += 4;
			}

			if (path[indexPath].x > position.x / app->map->data.tileWidth)
			{
				if ((currentAnim != &runRightAnim))
				{
					runRightAnim.Reset();
					currentAnim = &runRightAnim;
				}

				if (currentAnim != &deathRightAnim)
					position.x += 300.0f * dt;
			}

			if (path[indexPath].x < position.x / app->map->data.tileWidth)
			{
				if ((currentAnim != &runLeftAnim))
				{
					runLeftAnim.Reset();
					currentAnim = &runLeftAnim;
				}

				if (currentAnim != &deathLeftAnim)
					position.x -= 300.0f * dt;
			}
		}
	}

	return false;
}

void Wolf::HandleCollisions(float dt)
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint wolfPosTop = app->map->WorldToMap(position.x + 2, position.y - 3);
	iPoint wolfPosBottom = app->map->WorldToMap(position.x + 23, position.y + 26);

	iPoint wolfPosRight = app->map->WorldToMap(position.x + 45 + 10, position.y + 13);
	iPoint wolfPosLeft = app->map->WorldToMap(position.x - 10, position.y + 13);

	iPoint wolfPosTopRight = app->map->WorldToMap(position.x + 45, position.y - 3);
	iPoint playerPosTopLeft = app->map->WorldToMap(position.x - 3, position.y - 3);

	iPoint wolfPosBottomRight = app->map->WorldToMap(position.x + 45, position.y + 26);
	iPoint wolfPosBottomLeft = app->map->WorldToMap(position.x - 3, position.y + 26);


	while (layer != NULL)
	{
		if (layer->data->name == "HitBoxes")
		{
			// Here we get the surrounders player's tiles
			uint wolfIdBottom = layer->data->Get(wolfPosBottom.x, wolfPosBottom.y);

			uint wolfIdRight = layer->data->Get(wolfPosRight.x, wolfPosRight.y);
			uint wolfIdLeft = layer->data->Get(wolfPosLeft.x, wolfPosLeft.y);

			uint wolfIdBottomRight = layer->data->Get(wolfPosBottomRight.x, wolfPosBottomRight.y);
			uint wolfIdBottomLeft = layer->data->Get(wolfPosBottomLeft.x, wolfPosBottomLeft.y);

			// Detect platform collision and ignore it if hes jumping upwards
			if (wolfIdBottom == 1162 )
				blockFall = true;
			
			else if ((wolfIdBottom == 1161) && (wolfIdRight != 1161))
				blockFall = true;
			
			else
				blockFall = false;
			
			// Block right movement
			if (wolfIdRight == 1164)
			{
				position.x -= 100 * dt;
 				speedX = -speedX;

				if (currentAnim != &walkLeftAnim)
				{
					walkLeftAnim.Reset();
					currentAnim = &walkLeftAnim;
				}
				currentState = EnemyState::PATROL;
			}

			if (wolfIdLeft == 1164)
			{
				position.x += 100 * dt;
				speedX = -speedX;

				if (currentAnim != &walkRightAnim)
				{
					walkRightAnim.Reset();
					currentAnim = &walkRightAnim;
				}
				currentState = EnemyState::PATROL;
			}

			break;
		}

		layer = layer->next;
	}
}

bool Wolf::Load(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();
	currentState = (EnemyState)node.child("current_state").attribute("value").as_int();

	return true;
}

bool Wolf::Save(pugi::xml_node& node)
{
	pugi::xml_node wolf = node.append_child("position");
	wolf.append_attribute("x").set_value(position.x);
	wolf.append_attribute("y").set_value(position.y);

	pugi::xml_node state = node.append_child("current_state");
	state.append_attribute("value").set_value((int)currentState);

	return true;
}

void Wolf::Attack()
{
}

void Wolf::Draw()
{
	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());

	if (app->map->viewHitboxes == true)
	{
		app->pathFinding->DrawPath(path);
	}
}