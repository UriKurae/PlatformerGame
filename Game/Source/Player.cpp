#include "Log.h"

#include "Player.h"
#include "Render.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Window.h"
#include "Audio.h"

#include "Map.h"
#include "List.h"


#include "SDL/include/SDL.h"


Player::Player()
{
	name.Create("player");

	LoadPushbacks();
	
}

Player::~Player()
{
}

bool Player::Load(pugi::xml_node& playerNode)
{
	loadedGame = true;
	position.x = playerNode.child("position").attribute("positionX").as_float();
	position.y = playerNode.child("position").attribute("positionY").as_float();
	currentLevel = playerNode.child("current_level").attribute("value").as_int();

	return true;
}


bool Player::Save(pugi::xml_node& playerNode)
{
	pugi::xml_node player = playerNode.append_child("position");
	player.append_attribute("positionX").set_value(position.x);
	player.append_attribute("positionY").set_value(position.y);

	pugi::xml_node currLevel = playerNode.append_child("current_level");
	currLevel.append_attribute("value").set_value(currentLevel);
	return true;
}


bool Player::Start()
{
	LOG("Loading player textures");

	// Load the spritesheet for the player
	if (this->active == true)
	{
		texture = app->tex->Load("Assets/textures/Player/adventurer-Sheet.png");

		currentAnim = &idleAnim;
		speedX = 0.35f;
		speedY = 1.5f;
		minSpeedY = -0.05f;
		godMode = false;

	}

	return true;
}

bool Player::Update(float dt)
{
	// Detect player's input

	if ((speedY == minSpeedY) && (blockFall == true)) speedY = 1.5f;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN)
	{
		godMode = !godMode;
		blockFall = !blockFall;
	}
	if (godMode == true)
	{
		
		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		{
			position.y -= 3.0f;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			position.y += 3.0f;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		if ((currentAnim != &runRightAnim) && (jump == false))
		{
			runRightAnim.Reset();
			currentAnim = &runRightAnim;
			
		}
		if (jump == true)
		{
			currentAnim = &jumpRightAnim;
		}
		if (godMode)
		{
			position.x += 3.0f;
		}
		if (blockRightMovement == false)
		{
			position.x += speedX;
		}
		direction = "right";
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		if ((currentAnim != &runLeftAnim) && (jump == false))
		{
			runLeftAnim.Reset();
			currentAnim = &runLeftAnim;
			
		}
		if(jump == true)
		{
			currentAnim = &jumpLeftAnim;
		}
		if (godMode)
		{
			position.x -= 3.0f;
		}
		if (blockLeftMovement == false)
		{
			position.x -= speedX;
		}
		direction = "left";
	}
	

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		
		if (currentAnim != &jumpRightAnim)
		{
			jumpRightAnim.Reset();
			currentAnim = &jumpRightAnim;
		}
		if (jumpsLeft == 1)
		{
			jumpRightAnim.Reset();
		}

		// We shall reset the velocity each time we can jump.
		if (jumpsLeft > 0)
		{
			speedY = 1.8f;
			jumpsLeft--;
			jump = true;
		}
	
	}

	if (jump == true)
	{
		Jump();
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE &&
		app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE && jump == false)
	{
		if (currentAnim != &idleAnim)
		{
			idleAnim.Reset();
			currentAnim = &idleAnim;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT && jump == false)
	{
		if (currentAnim != &idleAnim)
		{
			idleAnim.Reset();
			currentAnim = &idleAnim;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		if (currentAnim != &jumpLeftAnim)
		{
			jumpLeftAnim.Reset();
			currentAnim = &jumpLeftAnim;
		}
		direction = "left";
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		if (currentAnim != &jumpRightAnim)
		{
			jumpRightAnim.Reset();
			currentAnim = &jumpRightAnim;
		}
		direction = "right";
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE &&
		app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_IDLE &&
		app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE && isFalling == false
		)
	{
		if (currentAnim != &idleAnim)
		{
			idleAnim.Reset();
			currentAnim = &idleAnim;
		}
	}

	if (isFalling == true)
	{
		if (direction == "right" && currentAnim != &jumpRightAnim)
		{
			
			fallingRightAnim.Reset();
			currentAnim = &fallingRightAnim;
		}
		if (direction == "left" && currentAnim != &jumpLeftAnim)
		{
			fallingLeftAnim.Reset();
			currentAnim = &fallingLeftAnim;
		}
	}
	
	if (blockFall == false && godMode == false)
	{
		position.y += gravity;
		isFalling = true;
		
	}

	if (godMode == false)
	{
		OnCollision();
	}
	currentAnim->Update();

	return true;
}

bool Player::PostUpdate()
{

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture,position.x,position.y, &rect);
	
		

	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);

	return true;
}


void Player::OnCollision()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint playerPosTop = app->map->WorldToMap(position.x + 2, position.y - 3);
	iPoint playerPosBottom = app->map->WorldToMap(position.x + 8, position.y + 30);
	
	iPoint playerPosRight = app->map->WorldToMap(position.x + 10, position.y + 15);
	iPoint playerPosLeft = app->map->WorldToMap(position.x - 3, position.y + 15);
	
	iPoint playerPosTopRight = app->map->WorldToMap(position.x + 30, position.y - 3);
	iPoint playerPosTopLeft = app->map->WorldToMap(position.x - 3, position.y - 3);
	
	iPoint playerPosBottomRight = app->map->WorldToMap(position.x + 30, position.y + 30);
	iPoint playerPosBottomLeft = app->map->WorldToMap(position.x - 3, position.y + 30);



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

			uint playerMidTile = layer->data->Get(playerPosTop.x + 15, playerPosTop.y + 20);

			// Block the player speed when collides with ceiling
			if (playerIdTop == 1161)
			{
				speedY = 0;
			}

			// Detect platform collision and ignore it if hes jumping upwards
			if (playerIdBottom == 1162 && upwards == false)
			{
				blockFall = true;
				isFalling = false;
				jump = false;
				jumpsLeft = 2;
			}
			else if (playerIdBottom == 1161 && upwards == false)
			{
				blockFall = true;
				jump = false;
				isFalling = false;
				jumpsLeft = 2;
			}
			// Check if player is colliding with the ground
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


void Player::Jump()
{
	if (speedY > 0)
	{
		position.y -= speedY;
		speedY -= 0.01f;
		upwards = true;
	}
	if (speedY <= 0)
	{
		upwards = false;
		blockFall = false;
	}

}


void Player::LoadPushbacks()
{
	// Idle animation
	idleAnim.PushBack({ 13,7,19,29 });
	idleAnim.PushBack({ 65,5,17,30 });
	idleAnim.PushBack({ 114,5,19,30 });
	idleAnim.PushBack({ 162,6,20,29 });

	idleAnim.speed = 0.008f;
	idleAnim.loop = true;

	// Run Right Animation
	runRightAnim.PushBack({ 66,44,20,28 });
	runRightAnim.PushBack({ 115,45,20,27 });
	runRightAnim.PushBack({ 165,47,20,25 });
	runRightAnim.PushBack({ 216,44,23,28 });
	runRightAnim.PushBack({ 266,45,20,27 });
	runRightAnim.PushBack({ 315,47,20,25 });
	

	runRightAnim.speed = 0.02f;
	runRightAnim.loop = true;

	// Left right Animation
	runLeftAnim.PushBack({ 618,45,20,28 });
	runLeftAnim.PushBack({ 568,46,20,27 });
	runLeftAnim.PushBack({ 518,48,20,25 });
	runLeftAnim.PushBack({ 465,45,23,28 });
	runLeftAnim.PushBack({ 419,46,20,27 });
	runLeftAnim.PushBack({ 368,49,20,25 });

	runLeftAnim.speed = 0.02f;
	runLeftAnim.loop = true;


	// Jump right animation
	jumpRightAnim.PushBack({15, 86, 20, 24});
	jumpRightAnim.PushBack({65, 88, 20, 22});
	jumpRightAnim.PushBack({117, 81, 19, 27});
	jumpRightAnim.PushBack({164, 79, 21, 23});
	jumpRightAnim.PushBack({218, 82, 21, 23});
	jumpRightAnim.PushBack({264, 84, 23, 17});
	jumpRightAnim.PushBack({320, 84, 18, 20});
	jumpRightAnim.PushBack({14, 124, 23, 17});
	jumpRightAnim.PushBack({68, 112, 17, 31});
	jumpRightAnim.PushBack({118, 113, 18, 30});

	jumpRightAnim.speed = 0.025f;
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
	
	jumpLeftAnim.speed = 0.025f;
	jumpLeftAnim.loop = false;

	// Falling anim right

	fallingRightAnim.PushBack({ 68,111,17,31 });
	fallingRightAnim.PushBack({ 117,112,17,30 });

	// Falling anim left

	fallingLeftAnim.PushBack({ 619,112,17,31 });
	fallingLeftAnim.PushBack({ 569,112,17,30 });
}


Point<float> Player::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;

	return position;

}

Point<float> Player::GetPosition()
{
	return position;
}
