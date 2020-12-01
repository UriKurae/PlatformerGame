#include "App.h"
#include "Ghost.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"

Ghost::Ghost(iPoint pos) : Enemy(pos)
{
	// Idle animation
	idleAnim.PushBack({4,106, 35, 25});
	idleAnim.PushBack({45,104, 35, 27});
	idleAnim.PushBack({102,85, 35, 29});
	idleAnim.PushBack({124,99, 35, 32});
	idleAnim.PushBack({202,99, 35, 32});
	idleAnim.PushBack({241,102, 35, 30});
	idleAnim.PushBack({319,107, 35, 24});


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
