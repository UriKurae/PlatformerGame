#include "Log.h"

#include "Player.h"
#include "Render.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"

#include "Map.h"
#include "List.h"


#include "SDL/include/SDL.h"


Player::Player()
{
	name.Create("player");


	// Idle animation
	idleAnim.PushBack({66,195,65,92});
	idleAnim.PushBack({0,196,65,92});

	idleAnim.speed = 0.0007f;

	// Run Right Animation
	runRightAnim.PushBack({ 151,200,63,89 });
	runRightAnim.PushBack({ 216,200,64,90 });
	runRightAnim.PushBack({ 281,200,65,90 });
	runRightAnim.PushBack({ 151,291,63,90 });
	runRightAnim.PushBack({ 216,291,64,89 });
	runRightAnim.PushBack({ 281,291,69,91 });
	runRightAnim.PushBack({ 150,384,69,89 });
	runRightAnim.PushBack({ 221,384,68,88 });
	runRightAnim.PushBack({ 290,383,68,90 });
	runRightAnim.PushBack({ 151,475,68,89 });
	runRightAnim.PushBack({ 221,475,63,90 });

	runRightAnim.speed = 0.01f;
	runRightAnim.loop = true;

	// Left right Animation
	runLeftAnim.PushBack({ 437,199,63,90 });
	runLeftAnim.PushBack({ 502,200,64,90 });
	runLeftAnim.PushBack({ 568,200,63,89 });
	runLeftAnim.PushBack({ 433,292,69,89 });
	runLeftAnim.PushBack({ 503,292,64,89 });
	runLeftAnim.PushBack({ 568,292,63,89 });
	runLeftAnim.PushBack({ 424,383,68,90 });
	runLeftAnim.PushBack({ 495,383,68,89 });
	runLeftAnim.PushBack({ 566,383,67,90 });
	runLeftAnim.PushBack({ 499,474,63,90 });
	runLeftAnim.PushBack({ 563,474,68,90 });

	runLeftAnim.speed = 0.01f;
	runLeftAnim.loop = true;
	

	// Jump animation
	jumpAnim.PushBack({437,92,68,93});
}

Player::~Player()
{
}

bool Player::Start()
{
	LOG("Loading player textures");

	// Load the spritesheet for the player
	texture = app->tex->Load("Assets/textures/Player/p1_spritesheetWithRun.png");

	
	currentAnim = &idleAnim;

	return true;
}

bool Player::Update(float dt)
{
	// Detect player's input
	

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		if (currentAnim != &runRightAnim)
		{
			runRightAnim.Reset();
			currentAnim = &runRightAnim;

		}
		if (blockRightMovement == false)
		{
			position.x += speedX;
		}

	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		if (currentAnim != &runLeftAnim)
		{
			runLeftAnim.Reset();
			currentAnim = &runLeftAnim;

		}

		if (blockLeftMovement == false)
		{
			position.x -= speedX;
		}

	}
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		if (currentAnim != &jumpAnim)
		{
			jumpAnim.Reset();
			currentAnim = &jumpAnim;

		}
		jump = true;
		grounded = false;
	}

	if (jump == true)
	{
		position.y -= speedY - gravity;
		speedY -= 0.005f;
		if (speedY <= 0.5f) speedY = 0;

		
		
		/*if (position.y <= 468)
		{
		position.y -= (speedY + gravity);
		speedY -= gravity;

		}
		else
		{
			jump = false;
			speedY = 10.0f;
		}*/
		
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE &&
		app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE)
	{
		if (currentAnim != &idleAnim)
		{
			idleAnim.Reset();
			currentAnim = &idleAnim;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		if (currentAnim != &idleAnim)
		{
			idleAnim.Reset();
			currentAnim = &idleAnim;
		}
	}


	currentAnim->Update();

	if (blockFall == false)
	{
		position.y += gravity;
	}

	OnCollision();

	return true;
}

bool Player::PostUpdate()
{

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture,position.x,position.y, &rect);
	
	app->render->camera.x =-(position.x-600);
	app->render->camera.y = 0;
	

	return true;
}

bool Player::CleanUp()
{
	return true;
}


void Player::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;

}

void Player::OnCollision()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint playerPosVert = app->map->WorldToMap(position.x, position.y + 93);
	iPoint playerPosRight = app->map->WorldToMap(position.x + 65, position.y + 46);
	iPoint playerPosLeft = app->map->WorldToMap(position.x - 3, position.y + 46);

	while (layer != NULL)
	{

		if (layer->data->name == "hitboxes")
		{
			uint playerIdVert = layer->data->Get(playerPosVert.x, playerPosVert.y);
			uint playerIdRight = layer->data->Get(playerPosRight.x, playerPosRight.y);
			uint playerIdLeft = layer->data->Get(playerPosLeft.x, playerPosLeft.y);


			if (playerIdVert == 157)
			{
				blockFall = true;
				grounded = false;
			}
			else
			{
				blockFall = false;
				grounded = true;
			}

			if (playerIdRight == 157)
			{
				blockRightMovement = true;
			}
			else
			{
				blockRightMovement = false;
			}

			if (playerIdLeft == 157)
			{
				blockLeftMovement = true;
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
	int force = 10;
	grounded = false;
	//position.y -= speedY - 0.5f * gravity;
	

}