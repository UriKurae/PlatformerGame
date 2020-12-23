#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Player.h"
#include "RedHeart.h"
#include "ItemManager.h"
#include "Collisions.h"

RedHeart::RedHeart(iPoint pos) : Item(pos)
{
	name.Create("redheart");

	idleAnim.PushBack({ 121, 79, 16, 16 });
	idleAnim.PushBack({ 139, 79, 16, 16 });
	idleAnim.PushBack({ 157, 79, 16, 16 });
	idleAnim.PushBack({ 176, 79, 16, 16 });
	idleAnim.PushBack({ 121, 99, 16, 16 });
	idleAnim.PushBack({ 139, 99, 16, 16 });
	idleAnim.PushBack({ 157, 99, 16, 16 });
	idleAnim.PushBack({ 176, 99, 16, 16 });
	idleAnim.loop = true;
}

RedHeart::~RedHeart()
{
}

bool RedHeart::Start()
{
	this->active = true;
	texture = app->tex->Load("Assets/Textures/Collectibles/collectibles.png");
	collider = app->collisions->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::ITEM);
	currentAnim = &idleAnim;

	type = ItemType::HEART;

	return true;
}

bool RedHeart::Update(float dt)
{
	idleAnim.speed = 5.0f * dt;
	currentAnim->Update();

	return true;
}

void RedHeart::Draw()
{
	if(active)
		app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
}

bool RedHeart::CleanUp()
{
	this->active = false;
	this->collider->pendingToDelete = true;
	app->itemManager->DeleteItem(this);
	app->tex->UnLoad(texture);

	return true;
}
