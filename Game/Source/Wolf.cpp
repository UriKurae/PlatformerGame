#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"
#include "EnemyManager.h"
#include "Wolf.h"

Wolf::Wolf(iPoint pos) : Enemy(pos)
{
	name.Create("wolf");

	// Idle animation
	idleAnim.PushBack({13,124,38,24});
	idleAnim.PushBack({76,127,39,21});
	idleAnim.PushBack({139,127,39,21});
	idleAnim.PushBack({203,127,39,21});
	idleAnim.PushBack({266,127,39,21});
	idleAnim.PushBack({329,127,40,21});
	idleAnim.PushBack({266,127,39,21});
	idleAnim.PushBack({203,127,39,21});
	idleAnim.PushBack({139,127,39,21});
	idleAnim.PushBack({76,127,39,21});
	idleAnim.PushBack({13,124,38,24});
	idleAnim.loop = true;
	
	// Run animation 
	runAnim.PushBack({9,5,45,29});
	runAnim.PushBack({78,12,44,22});
	runAnim.PushBack({144,8,44,26});
	runAnim.PushBack({221,9,43,25});
	runAnim.PushBack({278,8,45,26});
	runAnim.loop = true;

	// Wlak animation
	walkAnim.PushBack({9,169,44,24});
	walkAnim.PushBack({74,168,45,25});
	walkAnim.PushBack({138,167,45,26});
	walkAnim.PushBack({202,168,45,25});
	walkAnim.PushBack({266,169,45,24});
	walkAnim.PushBack({330,169,44,24});
	walkAnim.PushBack({393,169,44,24});
	walkAnim.PushBack({457,168,45,25});
	walkAnim.PushBack({522,167,45,26});
	walkAnim.PushBack({586,168,45,25});
	walkAnim.PushBack({649,170,45,24});
	walkAnim.PushBack({713,170,44,24});
	walkAnim.loop = true;

	// Take damage animation
	hurtAnim.PushBack({ 12,207,45,29 });
	hurtAnim.PushBack({ 77,202,39,34 });
	hurtAnim.loop = false;
}

bool Wolf::Start()
{
	texture = app->tex->Load("Assets/textures/Wolf/Wolf.png");
	collider = app->collisions->AddCollider({ position.x - 2, position.y + 10, 38, 24 }, Collider::Type::ENEMY); // 10 stands for offset
	currentAnim = &idleAnim;
	life = 50;

	return true;
}

bool Wolf::Update(float dt)
{
	idleAnim.speed = 4.50f * dt;
	hurtAnim.speed = 10.0f * dt;


	if ((currentAnim != &idleAnim) && (hurtAnim.HasFinished()))
	{
		idleAnim.Reset();
		currentAnim = &idleAnim;
	}

	collider->SetPos(position.x, position.y);
	currentAnim->Update();

	return true;
}

bool Wolf::CleanUp()
{
	app->tex->UnLoad(texture);
	collider->pendingToDelete = true;
	app->enemyManager->DeleteEnemy(this);

	return true;
}

void Wolf::TakeDamage(int damage)
{
	life -= damage;	

	if (currentAnim != &hurtAnim)
	{
		hurtAnim.Reset();
		currentAnim = &hurtAnim;
	}
}

bool Wolf::Load(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("positionX").as_float();
	position.y = node.child("position").attribute("positionY").as_float();

	return true;
}

bool Wolf::Save(pugi::xml_node& node)
{
	pugi::xml_node executioner = node.append_child("position");
	executioner.append_attribute("x").set_value(position.x);
	executioner.append_attribute("y").set_value(position.y);

	return true;
}

void Wolf::Attack()
{
}

void Wolf::Draw()
{
	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
}
