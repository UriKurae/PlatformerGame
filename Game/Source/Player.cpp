#include "Log.h"

#include "Player.h"
#include "Render.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Window.h"

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
	position.x = playerNode.child("position").attribute("positionX").as_float();
	position.y = playerNode.child("position").attribute("positionY").as_float();

	return true;
}


bool Player::Save(pugi::xml_node& playerNode)
{
	pugi::xml_node player = playerNode.append_child("position");
	player.append_attribute("positionX").set_value(position.x);
	player.append_attribute("positionY").set_value(position.y);
	return true;
}


bool Player::Start()
{
	LOG("Loading player textures");

	// Load the spritesheet for the player
	texture = app->tex->Load("Assets/textures/Player/adventurer-Sheet.png");

	
	currentAnim = &idleAnim;

	return true;
}

bool Player::Update(float dt)
{
	// Detect player's input

	if ((speedY == minSpeedY) && (blockFall == true)) speedY = 1.5f;

	if (app->input->GetKey(SDL_SCANCODE_F9) == KeyState::KEY_DOWN)
	{
		godMode = !godMode;
		blockFall = !blockFall;
	}
	if (godMode == true)
	{
		
		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		{
			position.y -= 1.0f;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			position.y += 1.0f;
		}
	}

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
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
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
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		if (currentAnim != &jumpAnim)
		{
			jumpAnim.Reset();
			currentAnim = &jumpAnim;

		}
		// If jump is false, we shall reset the velocity for the next jump.
		if (jump == false) speedY = 1.8f;
		 jump = true;
	}

	if (jump == true)
	{
		Jump();
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

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT &&
		app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		currentAnim = &jumpAnim;

	}


	currentAnim->Update();

	
		if (blockFall == false)
		{
			position.y += 1.7f * gravity;
		}

		OnCollision();
	

	return true;
}

bool Player::PostUpdate()
{

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture,position.x,position.y, &rect);
	
	//app->render->camera.x = -(position.x - 600);
	//app->render->camera.y = 0;
	

	return true;
}

bool Player::CleanUp()
{
	return true;
}


void Player::OnCollision()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint playerPosTop = app->map->WorldToMap(position.x + 2, position.y - 3);
	iPoint playerPosBottom = app->map->WorldToMap(position.x + 32, position.y + 93);
	
	iPoint playerPosRight = app->map->WorldToMap(position.x + 62, position.y + 46);
	iPoint playerPosLeft = app->map->WorldToMap(position.x - 3, position.y + 46);
	
	iPoint playerPosTopRight = app->map->WorldToMap(position.x + 62, position.y - 3);
	iPoint playerPosTopLeft = app->map->WorldToMap(position.x - 3, position.y - 3);
	
	iPoint playerPosBottomRight = app->map->WorldToMap(position.x + 62, position.y + 93);
	iPoint playerPosBottomLeft = app->map->WorldToMap(position.x - 3, position.y + 93);



	while (layer != NULL)
	{

		if (layer->data->name == "hitboxes")
		{
			uint playerIdTop = layer->data->Get(playerPosTop.x, playerPosTop.y);
			uint playerIdBottom = layer->data->Get(playerPosBottom.x, playerPosBottom.y);

			uint playerIdRight = layer->data->Get(playerPosRight.x, playerPosRight.y);
			uint playerIdLeft = layer->data->Get(playerPosLeft.x, playerPosLeft.y);

			uint playerIdTopRight = layer->data->Get(playerPosTopRight.x, playerPosTopRight.y);
			uint playerIdTopLeft = layer->data->Get(playerPosTopLeft.x, playerPosTopLeft.y);

			uint playerIdBottomRight = layer->data->Get(playerPosBottomRight.x, playerPosBottomRight.y);
			uint playerIdBottomLeft = layer->data->Get(playerPosBottomLeft.x, playerPosBottomLeft.y);

			if (playerIdTop == 157 || playerIdTopRight == 157 || playerIdTopLeft == 157)
			{
				speedY = 0;
			}
			if (playerIdBottom == 157 || (playerIdBottom != 157) && (playerIdBottomLeft == 157))
			{
				blockFall = true;
				jump = false;
			}
			else
			{
				blockFall = false;
			}

			if (playerIdBottom != 157 && playerIdBottomRight == 157)
			{
				blockFall = true;
				blockRightMovement = true;
			}
			else if (playerIdRight == 157)
			{
				blockRightMovement = true;
			}
			else
			{
				blockRightMovement = false;
			}
			
			/*if (playerIdRight == 157 ||	 (playerIdRight != 157) && (playerIdTopRight == 157))
			{
				blockRightMovement = true;
			}
			else
			{
				blockRightMovement = false;
			}*/

			
			
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


void Player::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;

}

void Player::Jump()
{
	if (speedY > 0)
	{
		position.y -= speedY;
		speedY -= 0.003f;
	}
	if (speedY <= 0)
	{
		blockFall = false;
		/*position.y -= speedY;
		speedY -= 0.003f;
		
		if (speedY > minSpeedY) speedY = minSpeedY;*/
	}
}


void Player::LoadPushbacks()
{
	// Idle animation
	idleAnim.PushBack({ 13,7,19,29 });
	idleAnim.PushBack({ 65,5,17,30 });
	idleAnim.PushBack({ 114,5,19,30 });
	idleAnim.PushBack({ 162,6,20,29 });

	idleAnim.speed = 0.0007f;

	// Run Right Animation
	runRightAnim.PushBack({ 66,44,20,28 });
	runRightAnim.PushBack({ 115,45,20,27 });
	runRightAnim.PushBack({ 165,47,20,25 });
	runRightAnim.PushBack({ 216,44,23,28 });
	runRightAnim.PushBack({ 266,45,20,27 });
	runRightAnim.PushBack({ 315,47,20,25 });
	

	runRightAnim.speed = 0.01f;
	runRightAnim.loop = true;

	// Left right Animation
	runLeftAnim.PushBack({ 618,45,20,28 });
	runLeftAnim.PushBack({ 568,46,20,27 });
	runLeftAnim.PushBack({ 518,48,20,25 });
	runLeftAnim.PushBack({ 465,45,23,28 });
	runLeftAnim.PushBack({ 419,46,20,27 });
	runLeftAnim.PushBack({ 368,49,20,25 });

	runLeftAnim.speed = 0.01f;
	runLeftAnim.loop = true;


	// Jump animation
	jumpAnim.PushBack({ 437,92,68,93 });

}
