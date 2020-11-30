#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Scene1.h"

Scene1::Scene1()
{
	name.Create("scene1");
}

Scene1::~Scene1()
{
}

bool Scene1::Start()
{
	sky = app->tex->Load("Assets/textures/sky.png");
	sea = app->tex->Load("Assets/textures/sea.png");
	clouds = app->tex->Load("Assets/textures/clouds.png");

	//playerStartPosition() 
	return true;
}

bool Scene1::Update(float dt)
{

	return true;
}

bool Scene1::Draw()
{
	return true;
}

bool Scene1::CleanUp()
{
	return true;
}

bool Scene1::RestartPlayerPosition()
{
	return true;
}

int Scene1::CheckWin()
{
	return 1;
}
