#include "App.h"
#include "Bird.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"

Bird::Bird(iPoint pos)
{
	position = pos;
}

Bird::~Bird()
{
}

bool Bird::Start()
{
	
	texture = app->tex->Load("Assets/textures/Executioner/executioner.png");
	collider = app->collisions->AddCollider({ position.x - 2, position.y + 10, 37, 80 }, Collider::TYPE::ENEMY);

	return true;
}

bool Bird::Update(float dt)
{
	collider->SetPos(position.x - 2, position.y + 10);

	return true;
}

void Bird::Attack()
{
}

void Bird::Draw()
{

	if (currentAnim != nullptr)
	{
		app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
	}

}
