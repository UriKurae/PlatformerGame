#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Item.h"

Item::Item(iPoint pos)
{
	position = pos;
}

Item::~Item()
{
}

bool Item::Start()
{
	texture = app->tex->Load("Assets/Textures/Coins/gems.png");

	return false;
}

bool Item::Update(float dt)
{
	return true;
}

void Item::Draw()
{
}

bool Item::CleanUp()
{
	return true;
}

ItemType Item::GetType()
{
	return type;
}
