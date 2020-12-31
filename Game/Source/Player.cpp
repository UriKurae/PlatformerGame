#include "App.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Player.h"
#include "Map.h"
#include "EntityManager.h"

#include "List.h"
#include "Log.h"

#include <math.h>
#include "SDL/include/SDL.h"


Player::Player(iPoint pos) : Entity(pos)
{
	name.Create("player");
	
	// Idle animation
	idleRightAnim.PushBack({ 13,7,19,29 });
	idleRightAnim.PushBack({ 65,5,17,30 });
	idleRightAnim.PushBack({ 114,5,19,30 });
	idleRightAnim.PushBack({ 162,6,20,29 });
	idleRightAnim.loop = true;

	// Idle animation
	idleLeftAnim.PushBack({ 671,7,19,29 });
	idleLeftAnim.PushBack({ 621,5,17,30 });
	idleLeftAnim.PushBack({ 570,5,19,30 });
	idleLeftAnim.PushBack({ 521,6,20,29 });
	idleLeftAnim.loop = true;

	// Run Right Animation
	runRightAnim.PushBack({ 66,44,20,28 });
	runRightAnim.PushBack({ 115,45,20,27 });
	runRightAnim.PushBack({ 165,47,20,25 });
	runRightAnim.PushBack({ 216,44,23,28 });
	runRightAnim.PushBack({ 266,45,20,27 });
	runRightAnim.PushBack({ 315,47,20,25 });
	runRightAnim.loop = true;

	// Left right Animation
	runLeftAnim.PushBack({ 618,45,20,28 });
	runLeftAnim.PushBack({ 568,46,20,27 });
	runLeftAnim.PushBack({ 518,48,20,25 });
	runLeftAnim.PushBack({ 465,45,23,28 });
	runLeftAnim.PushBack({ 419,46,20,27 });
	runLeftAnim.PushBack({ 368,49,20,25 });
	runLeftAnim.loop = true;

	// Jump right animation
	jumpRightAnim.PushBack({ 15, 86, 20, 24 });
	jumpRightAnim.PushBack({ 65, 88, 20, 22 });
	jumpRightAnim.PushBack({ 117, 81, 19, 27 });
	jumpRightAnim.PushBack({ 164, 79, 21, 23 });
	jumpRightAnim.PushBack({ 218, 82, 21, 23 });
	jumpRightAnim.PushBack({ 264, 84, 23, 17 });
	jumpRightAnim.PushBack({ 320, 84, 18, 20 });
	jumpRightAnim.PushBack({ 14, 124, 23, 17 });
	jumpRightAnim.PushBack({ 68, 112, 17, 31 });
	jumpRightAnim.PushBack({ 118, 113, 18, 30 });
	jumpRightAnim.loop = false;

	// Jump left animation
	jumpLeftAnim.PushBack({ 620,89,20,22 });
	jumpLeftAnim.PushBack({ 569,82,19,27 });
	jumpLeftAnim.PushBack({ 520,80,21,23 });
	jumpLeftAnim.PushBack({ 472,83,15,20 });
	jumpLeftAnim.PushBack({ 418,85,23,17 });
	jumpLeftAnim.PushBack({ 367,85,18,20 });
	jumpLeftAnim.PushBack({ 668,125,23,17 });
	jumpLeftAnim.PushBack({ 620,113,17,31 });
	jumpLeftAnim.PushBack({ 570,114,17,30 });
	jumpLeftAnim.loop = false;

	// Falling anim right
	fallingRightAnim.PushBack({ 68,111,17,31 });
	fallingRightAnim.PushBack({ 117,112,17,30 });
	fallingRightAnim.loop = false;

	// Falling anim left
	fallingLeftAnim.PushBack({ 619,112,17,31 });
	fallingLeftAnim.PushBack({ 569,112,17,30 });
	fallingLeftAnim.loop = false;

	// Attack facing right animation
	attackRightDownUpAnim.PushBack({ 7, 236, 27, 22 });
	attackRightDownUpAnim.PushBack({ 58, 238, 25, 20 });
	attackRightDownUpAnim.PushBack({ 115, 222, 34, 36 });
	attackRightDownUpAnim.PushBack({ 165, 222, 27, 36 });
	attackRightDownUpAnim.PushBack({ 215, 226, 19, 32 });
	attackRightDownUpAnim.loop = false;

	// Atack facing left animation
	attackLeftDownUpAnim.PushBack({ 671, 237, 27, 22 });
	attackLeftDownUpAnim.PushBack({ 622, 239, 25, 20 });
	attackLeftDownUpAnim.PushBack({ 556, 223, 34, 36 });
	attackLeftDownUpAnim.PushBack({ 513, 223, 27, 36 });
	attackLeftDownUpAnim.PushBack({ 471, 227, 19, 32 });
	attackLeftDownUpAnim.loop = false;

	// Dodging Animation Right
	dodgingToRight.PushBack({218,268,20,26});

	// Dodging Animation Left
	dodgingToLeft.PushBack({466,269,20,26});
	
	Start();

}

Player::~Player()
{
}

bool Player::Load(pugi::xml_node& playerNode)
{
	loadedGame = true;
	position.x = playerNode.child("position").attribute("x").as_float();
	position.y = playerNode.child("position").attribute("y").as_float();

	savedPosition = position;

	return true;
}


bool Player::Save(pugi::xml_node& playerNode)
{
	pugi::xml_node player = playerNode.append_child("position");
	player.append_attribute("x").set_value(position.x);
	player.append_attribute("y").set_value(position.y);

	return true;
}


bool Player::Start()
{
	LOG("Loading player textures");

	// Load the spritesheet for the player
	texture = app->tex->Load("Assets/Textures/Player/player.png");
	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/jump.wav");
	hitFx = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/hit.wav");
	dodgingFx = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/dodging.wav");
	pickGemFx = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/pick_gem.wav");
	pickHeart = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/heart_sound.wav");

	dodgingCooldown = 0;
	dodgingTime = 0;

	gemsAchieved = 0;

	lifes = 3;

	speedX = 250.0f;
	speedY = 500.0f;
	gravity = 250.0f;
	jump = false;
	godMode = false;
	blockCamera = false;
	direction = "right";
	
	currentAnim = &idleRightAnim;
	collider = app->entityManager->AddCollider({ (int)position.x + 4, (int)position.y + 5, 10, 22 }, Collider::Type::PLAYER);


	return true;
}

bool Player::Update(float dt)
{
	idleLeftAnim.speed = 5.0f * dt;
	idleRightAnim.speed = 5.0f * dt;

	// Detect player's input
	HandleInput(dt);
		
	if (jump == true)
		Jump(dt);

	if ((isFalling == true) && (isDodging == false))
	{
		if ((direction == "right") && (currentAnim != &jumpRightAnim))
		{
			fallingRightAnim.Reset();
			currentAnim = &fallingRightAnim;
		}

		if ((direction == "left") && (currentAnim != &jumpLeftAnim))
		{
			fallingLeftAnim.Reset();
			currentAnim = &fallingLeftAnim;
		}
	}
	
	if ((blockFall == false) && (godMode == false) && (dt < 2))
	{
		position.y += gravity * dt;
		isFalling = true;
	}

	if (godMode == false)
	{
		OnCollision();
	}

	currentAnim->Update();

	if (direction == "right")
		collider->SetPos(position.x + 8, position.y + 5);

	else if (direction == "left")
		collider->SetPos(position.x + 4, position.y + 5);

	if (blockCamera == false)
		CameraFollow();
	

	return true;
}

void Player::Draw()
{
	// Draw player texture
	app->render->DrawTexture(texture,position.x,position.y, &currentAnim->GetCurrentFrame());
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	gemsAchieved = 0;
	app->entityManager->DeleteEntity(this);

	return true;
}

void Player::HandleInput(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN)
	{
		godMode = !godMode;
		blockFall = !blockFall;
	}


	if (godMode == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		{
			position.y -= 300.0f * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			position.y += 300.0f * dt;
		}
	}
	
	if (blockCamera == false)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT && (isDodging == false))
		{
			if ((currentAnim != &runRightAnim) && (jump == false))
			{
				runRightAnim.speed = 8.0f * dt;
				runRightAnim.Reset();
				currentAnim = &runRightAnim;
			}

			if (jump == true)
			{
				jumpRightAnim.speed = 15.0f * dt;
				currentAnim = &jumpRightAnim;
			}

			if (godMode)
			{
				position.x += 300.0f * dt;
			}

			if (blockRightMovement == false)
			{
				position.x += speedX * dt;
			}

			direction = "right";
		}

		if ((app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_DOWN) && (dodgingCooldown <= 0))
		{
			dodgingCooldown = 1500 * dt;
			dodgingTime = 800 * dt;
			isDodging = true;
			app->audio->PlayFx(dodgingFx);
		}

		if (dodgingTime > 0)
		{
			dodgingTime -= 50 * dt;

			if (direction == "right")
			{
				if (currentAnim != &dodgingToRight)
				{
					dodgingToRight.Reset();
					currentAnim = &dodgingToRight;
				}

				if (blockRightMovement == false)
					position.x += 400.0f * dt;
			}
			else if (direction == "left")
			{
				if (currentAnim != &dodgingToLeft)
				{
					dodgingToLeft.Reset();
					currentAnim = &dodgingToLeft;
				}

				if (blockLeftMovement == false)
					position.x -= 400.0f * dt;
			}
		}
		else
		{
			isDodging = false;
			dodgingCooldown -= 50 * dt;
		}

		if ((app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) && (isDodging == false))
		{
			if ((currentAnim != &runLeftAnim) && (jump == false))
			{
				runLeftAnim.speed = 8.0f * dt;
				runLeftAnim.Reset();
				currentAnim = &runLeftAnim;
			}

			if (jump == true)
			{
				jumpLeftAnim.speed = 15.0f * dt;
				currentAnim = &jumpLeftAnim;
			}

			if (godMode)
			{
				position.x -= 300.0f * dt;
			}

			if (blockLeftMovement == false)
			{
				position.x -= speedX * dt;
			}

			direction = "left";
		}


		if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) && (godMode == false) && (isDodging == false))
		{
			if ((currentAnim != &jumpRightAnim) && (direction == "right"))
			{
				jumpRightAnim.speed = 13.0f * dt;
				jumpRightAnim.Reset();
				currentAnim = &jumpRightAnim;

				app->audio->PlayFx(jumpFx);
			}

			else if ((currentAnim != &jumpLeftAnim) && (direction == "left"))
			{
				jumpLeftAnim.speed = 13.0f * dt;
				jumpLeftAnim.Reset();
				currentAnim = &jumpLeftAnim;

				app->audio->PlayFx(jumpFx);
			}

			if (jumpsLeft == 1)
			{
				jumpRightAnim.Reset();
				jumpLeftAnim.Reset();

				app->audio->PlayFx(jumpFx);

			}

			// We shall reset the velocity each time we can jump.
			if (jumpsLeft > 0)
			{
				speedY = 400.0f;
				jumpsLeft--;
				jump = true;
			}
		}

		if ((app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE)
			&& (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE)
			&& (jump == false) && (isDodging == false))
		{
			if ((currentAnim != &idleRightAnim) && (currentAnim != &attackRightDownUpAnim) && (direction == "right"))
			{
				idleRightAnim.Reset();
				currentAnim = &idleRightAnim;
			}

			else if ((currentAnim != &idleLeftAnim) && (currentAnim != &attackLeftDownUpAnim) && (direction == "left"))
			{
				idleLeftAnim.Reset();
				currentAnim = &idleLeftAnim;
			}
		}


		if ((app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) &&
			(app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) &&
			(jump == false) && (isDodging == false))
		{
			if ((currentAnim != &idleRightAnim) && (direction == "right"))
			{
				idleRightAnim.Reset();
				currentAnim = &idleRightAnim;
			}

			else if ((currentAnim != &idleLeftAnim) && (direction == "left"))
			{
				idleLeftAnim.Reset();
				currentAnim = &idleLeftAnim;
			}
		}


		if ((app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) &&
			(app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) && (isDodging == false))
		{
			if (currentAnim != &jumpLeftAnim)
			{
				jumpLeftAnim.Reset();
				currentAnim = &jumpLeftAnim;
			}

			direction = "left";
		}


		if ((app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) &&
			(app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) && (isDodging == false))
		{
			if (currentAnim != &jumpRightAnim)
			{
				jumpRightAnim.Reset();
				currentAnim = &jumpRightAnim;
			}

			direction = "right";
		}


		if ((app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE) &&
			(app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_IDLE) &&
			(app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE) &&
			(isFalling == false) && (isDodging == false))
		{
			if ((currentAnim != &idleRightAnim) && (currentAnim != &attackRightDownUpAnim) && (direction == "right"))
			{
				idleRightAnim.Reset();
				currentAnim = &idleRightAnim;
			}

			else if ((currentAnim != &idleLeftAnim) && (currentAnim != &attackLeftDownUpAnim) && (direction == "left"))
			{
				idleLeftAnim.Reset();
				currentAnim = &idleLeftAnim;
			}
		}

		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (direction == "right")
			{
				if (currentAnim != &attackRightDownUpAnim)
				{
					attackRightDownUpAnim.speed = 14.0f * dt;
					currentAnim = &attackRightDownUpAnim;
				}
			}
			else if (direction == "left")
			{
				if (currentAnim != &attackLeftDownUpAnim)
				{
					attackLeftDownUpAnim.speed = 14.0f * dt;
					currentAnim = &attackLeftDownUpAnim;
				}
			}
			app->audio->PlayFx(hitFx);

			Attack();
		}

		if (attackRightDownUpAnim.HasFinished())
		{
			attackRightDownUpAnim.Reset();
			currentAnim = &idleRightAnim;
		}

		else if (attackLeftDownUpAnim.HasFinished())
		{
			attackLeftDownUpAnim.Reset();
			currentAnim = &idleLeftAnim;
		}
	}

}

void Player::CameraFollow()
{
	app->render->camera.x = -(position.x * (int)app->win->GetScale()) + app->render->cameraOffsetX;
	app->render->camera.y = -(position.y * (int)app->win->GetScale()) + app->render->cameraOffsetY;
}


void Player::OnCollision()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint playerPosTop = app->map->WorldToMap(position.x + 2, position.y - 3);
	iPoint playerPosBottom = app->map->WorldToMap(position.x + 7, position.y + 30);
	
	iPoint playerPosRight = app->map->WorldToMap(position.x + 10, position.y + 15);
	iPoint playerPosLeft = app->map->WorldToMap(position.x - 3, position.y + 15);
	
	iPoint playerPosTopRight = app->map->WorldToMap(position.x + 30, position.y - 3);
	iPoint playerPosTopLeft = app->map->WorldToMap(position.x - 3, position.y - 3);
	
	iPoint playerPosBottomRight = app->map->WorldToMap(position.x + 30, position.y + 30);
	iPoint playerPosBottomLeft = app->map->WorldToMap(position.x - 3, position.y + 30);
		
	iPoint playerPosMid = app->map->WorldToMap(position.x + 10, position.y + 15);


	while (layer != NULL)
	{
		if (layer->data->name == "HitBoxes")
		{
			// Here we get the surrounders player's tiles
			uint playerIdTop = layer->data->Get(playerPosTop.x, playerPosTop.y);
			uint playerIdBottom = layer->data->Get(playerPosBottom.x, playerPosBottom.y);

			uint playerIdRight = layer->data->Get(playerPosRight.x, playerPosRight.y);
			uint playerIdLeft = layer->data->Get(playerPosLeft.x, playerPosLeft.y);

			uint playerIdTopRight = layer->data->Get(playerPosTopRight.x, playerPosTopRight.y);
			uint playerIdTopLeft = layer->data->Get(playerPosTopLeft.x, playerPosTopLeft.y);

			uint playerIdBottomRight = layer->data->Get(playerPosBottomRight.x, playerPosBottomRight.y);
			uint playerIdBottomLeft = layer->data->Get(playerPosBottomLeft.x, playerPosBottomLeft.y);

			uint playerIdMidTile = layer->data->Get(playerPosMid.x, playerPosMid.y);

			// Block the player speed when collides with ceiling
			if (playerIdTop == 1161)
			{
				speedY = 0;
			}

			if ((playerIdMidTile == 1162) || (playerIdTop == 1162) || (playerIdLeft == 1162) || (playerIdRight == 1162) || 
				(playerIdTopRight == 1162) || (playerIdTopLeft == 1162) || (playerIdMidTile == 1161) || 
				(playerIdTop == 1161) || (playerIdLeft == 1161) || (playerIdRight == 1161))
			{
				isReachable = false;
			}
			else
			{
				isReachable = true;
			}

			// Detect platform collision and ignore it if hes jumping upwards
			if ((playerIdBottom == 1162) && (upwards == false))
			{
				blockFall = true;
				isFalling = false;
				jump = false;
				jumpsLeft = 2;
			}
			else if ((playerIdBottom == 1161) && (playerIdRight != 1161) && (upwards == false))
			{
				blockFall = true;
				jump = false;
				isFalling = false;
				jumpsLeft = 2;
			}
			else
			{
				blockFall = false;
			}
			
			// Check if the player is facing a collision with his right tile
			// If he does, we dont allow to walk to the right
			// If he doesn't, we allow to walk to the right
			if (playerIdRight == 1161)
			{
				blockRightMovement = true;
			}
			else if ((playerIdBottomRight == 1162) && (playerIdBottom != 1162))
			{
				blockRightMovement = true;
				jumpRightAnim.Reset();
			}
			else
			{
				blockRightMovement = false;
			}
			
			// Here we check if the player is facing a wall with his left tile
			// If he does, we dont allow to move to the left
			// If he doesn't, we allow to move to the left
			if (playerIdLeft == 1161)
			{
				blockLeftMovement = true;
			}
			else if ((playerIdBottomLeft == 1162) && (playerIdBottom != 1162))
			{
				blockLeftMovement = true;
				jumpRightAnim.Reset();
			}
			else
			{
				blockLeftMovement = false;
			}

			break;
		}

		layer = layer->next;
	}
}


void Player::Jump(float dt)
{
	if (speedY > 0)
	{
		position.y -= speedY * dt * 3.0f;
		speedY -= gravity * dt * 6.5f;
		upwards = true;
	}
	if (speedY <= 0)
	{
		isFalling = true;
		upwards = false;
		blockFall = false;	
	}
}

void Player::Attack()
{
	if (direction == "right")
		attackCollider = new Collider({ position.x + 20, position.y, 15, 25 }, Collider::Type::PLAYER_HIT);
	//attackCollider = app->entityManager->AddCollider({ position.x + 20, position.y, 15, 25 }, Collider::Type::PLAYER_HIT);

	else if (direction == "left")
		attackCollider = new Collider({ position.x - 10, position.y, 15, 25 }, Collider::Type::PLAYER_HIT);
	//attackCollider = app->entityManager->AddCollider({ position.x - 10, position.y, 15, 25 }, Collider::Type::PLAYER_HIT);

	ListItem<Enemy*>* currEnemy = app->entityManager->enemies.start;

	while (currEnemy != nullptr)
	{
		if (attackCollider->Intersects(currEnemy->data->collider->rect))
		{
			currEnemy->data->TakeDamage(20);
			LOG("%s has %i life", currEnemy->data->name.GetString(), currEnemy->data->life);
		}

		currEnemy = currEnemy->next;
	}
	
	delete attackCollider;
	attackCollider = nullptr;
}

void Player::PickItem(EntityType type)
{
	if (type == EntityType::GEM)
	{
		app->sceneManager->gemsPicked += 1;
		++gemsAchieved;
		app->audio->PlayFx(pickGemFx);
	}

	if (type == EntityType::HEART)
	{
		app->sceneManager->heartsPicked += 1;
		++lifes;
		app->audio->PlayFx(pickHeart);
	}
}

SDL_Rect Player::GetSize() const
{
	return collider->rect;
}

iPoint Player::SetPosition(int x, int y)
{
	position.x = x;
	position.y = y;

	return position;
}

iPoint Player::GetPosition()
{
	return position;
}

Collider* Player::GetCollider()
{
	return collider;
}

bool Player::GetReachable() const
{
	return isReachable;
}
