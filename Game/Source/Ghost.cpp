#include "App.h"
#include "Ghost.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"

Ghost::Ghost(iPoint pos) : Enemy(pos)
{
}

Ghost::~Ghost()
{
}

bool Ghost::Start()
{
	
	texture = app->tex->Load("Assets/textures/Executioner/executioner.png");
	collider = app->collisions->AddCollider({ position.x - 2, position.y + 10, 37, 80 }, Collider::TYPE::ENEMY);

	return true;
}

bool Ghost::Update(float dt)
{
	collider->SetPos(position.x - 2, position.y + 10);

	return true;
}

void Ghost::Attack()
{
}

void Ghost::Draw()
{

	if (currentAnim != nullptr)
	{
		app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
	}

}
