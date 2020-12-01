#include "App.h"
#include "Ghost.h"
#include "Render.h"
#include "Textures.h"
#include "Collisions.h"

Ghost::Ghost(iPoint pos) : Enemy(pos)
{
	// Idle animation
	idleAnim.PushBack({4, 106, 35, 25});
	idleAnim.PushBack({45, 104, 35, 27});
	idleAnim.PushBack({102, 85, 35, 29});
	idleAnim.PushBack({124, 99, 35, 32});
	idleAnim.PushBack({202, 99, 35, 32});
	idleAnim.PushBack({241, 102, 35, 30});
	idleAnim.PushBack({319, 107, 35, 24});
	idleAnim.loop = true;

	// Walk animation
	walkAnim.PushBack({4, 193, 35, 25});
	walkAnim.PushBack({43, 193, 35, 25});
	walkAnim.PushBack({81, 193, 35, 25});
	walkAnim.PushBack({119, 193, 35, 25});
	walkAnim.PushBack({158, 193, 35, 25});
	walkAnim.loop = true;

	// Take damage animation
	takeDamageAnim.PushBack({4, 149, 35, 25});
	takeDamageAnim.PushBack({45, 149, 35, 25});
	takeDamageAnim.PushBack({84, 149, 34, 25});
	takeDamageAnim.PushBack({120, 149, 35, 25});
	takeDamageAnim.loop = false;

	// Die animation
	dieAnim.PushBack({3, 61, 34, 25});
	dieAnim.PushBack({45, 59, 24, 30});
	dieAnim.PushBack({76, 60, 22, 28});
	dieAnim.PushBack({105, 61, 20, 26});
	dieAnim.PushBack({136, 68, 12, 14});


}

Ghost::~Ghost()
{
}

bool Ghost::Start()
{
	
	texture = app->tex->Load("Assets/textures/Executioner/executioner.png");
	collider = app->collisions->AddCollider({ position.x - 2, position.y + 10, 37, 80 }, Collider::Type::ENEMY);

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
