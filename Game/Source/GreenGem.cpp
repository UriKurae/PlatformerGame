#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "GreenGem.h"
#include "ItemManager.h"
#include "Player.h"
#include "Collisions.h"

GreenGem::GreenGem(iPoint pos) : Item(pos)
{
	name.Create("greengem");
	
	idleAnim.PushBack({ 2,2,10,15 });
	idleAnim.PushBack({ 18,2,10,15 });
	idleAnim.PushBack({ 50,2,10,15 });
	idleAnim.loop = true;

}

GreenGem::~GreenGem()
{
}

bool GreenGem::Start()
{
	this->active = true;

	texture = app->itemManager->texture;
	collider = app->collisions->AddCollider({ position.x, position.y, 10,15 }, Collider::Type::ITEM);
	currentAnim = &idleAnim;

	type = ItemType::GEM;

	return true;
}

bool GreenGem::Update(float dt)
{
	idleAnim.speed = 5.0f * dt;

	if (app->player->GetCollider()->Intersects(collider->rect))
	{
		app->player->PickItem(type);
		CleanUp();
	}

	currentAnim->Update();

	return true;
}

void GreenGem::Draw()
{
	if(active)
		app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
}

bool GreenGem::CleanUp()
{
	this->active = false;
	this->collider->pendingToDelete = true;
	app->itemManager->DeleteItem(this);

	return true;
}
