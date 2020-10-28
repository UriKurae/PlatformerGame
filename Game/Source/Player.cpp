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

	// Run Animation
	runAnim.PushBack({ 6,1,65,91 });
	runAnim.PushBack({ 78,1,65,91 });
	runAnim.PushBack({ 146,0,71,92 });
	runAnim.PushBack({ 218,5,70,92 });
	runAnim.PushBack({ 1,99,70,91 });
	runAnim.PushBack({ 74,101,69,91});
	runAnim.PushBack({ 146,101,70,91});

	runAnim.speed = 0.01f;
	runAnim.loop = true;
	

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
	texture = app->tex->Load("Assets/textures/Player/p1_spritesheet.png");
	
	
	currentAnim = &idleAnim;

	return true;
}

bool Player::Update(float dt)
{


	// Detect player's input

	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{


		if (currentAnim != &runAnim)
		{
			runAnim.Reset();
			currentAnim = &runAnim;

		}
		
		position.x += speedX;

	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{


		if (currentAnim != &runAnim)
		{
			runAnim.Reset();
			currentAnim = &runAnim;

		}

		position.x -= speedX;

	}
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{

		if (currentAnim != &jumpAnim)
		{
			runAnim.Reset();
			currentAnim = &runAnim;

		}
		jump = true;
	}

	if (jump == true)
	{
		if (position.y <= 468)
		{
		position.y -= (speedY + gravity);
		speedY -= gravity;

		}
		else
		{
			jump = false;
			speedY = 10.0f;
		}
		
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

	currentAnim->Update();

	//position.y += gravity;
	
	if (OnCollision())
	{
		// This is the same as calculating the normal force the ground exerts the body
		// On result, we have the player with a resultant force of 0.
		position.y -= gravity;
	}
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

bool Player::OnCollision()
{
	bool ret = false;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	while (layer != NULL)
	{
		if ((layer->data->name == "hitboxes") && (layer->data->properties.GetProperty("Navigation")) == 0)
		{
			ret = true;
		}
		layer = layer->next;
	}




	return ret;
}
