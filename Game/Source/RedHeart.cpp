#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Player.h"
#include "RedHeart.h"
#include "EntityManager.h"
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
	idleAnim.loop = true;

	Start();
}

RedHeart::~RedHeart()
{
}

bool RedHeart::Start()
{
	this->active = true;
	texture = app->entityManager->collectiblesTexture;
	collider = app->entityManager->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::ITEM);
	currentAnim = &idleAnim;

	type = EntityType::HEART;

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
	//this->collider->pendingToDelete = true;
	app->entityManager->DeleteEntity(this);

	return true;
}

bool RedHeart::Load(pugi::xml_node& node)
{
	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();

	return true;
}

bool RedHeart::Save(pugi::xml_node& node)
{
	pugi::xml_node heart = node.append_child("position");
	heart.append_attribute("x").set_value(position.x);
	heart.append_attribute("y").set_value(position.y);

	return true;
}
