#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "GreenGem.h"
#include "EntityManager.h"
#include "Player.h"
#include "Collisions.h"

GreenGem::GreenGem(iPoint pos) : Item(pos)
{
	name.Create("greengem");
	
	idleAnim.PushBack({ 2,2,10,15 });
	idleAnim.PushBack({ 18,2,10,15 });
	idleAnim.PushBack({ 50,2,10,15 });
	idleAnim.loop = true;

	Start();
}

GreenGem::~GreenGem()
{
}

bool GreenGem::Start()
{
	this->active = true;

	texture = app->entityManager->collectiblesTexture;
	collider = app->entityManager->AddCollider({ position.x, position.y, 10,15 }, Collider::Type::ITEM);
	currentAnim = &idleAnim;

	type = EntityType::GEM;

	return true;
}

bool GreenGem::Update(float dt)
{
	idleAnim.speed = 5.0f * dt;
	currentAnim->Update();
	collider->SetPos(position.x, position.y);

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
	//this->collider->pendingToDelete = true;
	app->entityManager->DeleteEntity(this);

	return true;
}

bool GreenGem::Load(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();

	return true;
}

bool GreenGem::Save(pugi::xml_node& node)
{
	pugi::xml_node gem = node.append_child("position");
	gem.append_attribute("x").set_value(position.x);
	gem.append_attribute("y").set_value(position.y);

	return true;
}
