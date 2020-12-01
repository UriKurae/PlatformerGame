#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"
#include "Wolf.h"

Wolf::Wolf(iPoint pos) : Enemy(pos)
{


}

bool Wolf::Start()
{
	//texture = app->tex->Load();
	collider = app->collisions->AddCollider({ position.x - 2, position.y + 10, 37, 80 }, Collider::TYPE::ENEMY); // 10 stands for offset
	return true;
}

bool Wolf::Update(float dt)
{
	collider->SetPos(position.x, position.y);
	return true;
}

void Wolf::Attack()
{
}

void Wolf::Draw()
{

	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
}
