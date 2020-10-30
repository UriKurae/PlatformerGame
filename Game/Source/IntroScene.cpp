
#include "App.h"
#include "IntroScene.h"

#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"


IntroScene::IntroScene()
{
	
}

IntroScene::~IntroScene()
{

}

bool IntroScene::Awake(pugi::xml_node& config)
{

	return true;
}
// Load assets
bool IntroScene::Start()
{
	app->tex->Load("Assets/textures/sky2.png");

	return true;
}

bool IntroScene::Update(float dt)
{
	
	return true;
}

// Update: draw background
bool IntroScene::PostUpdate()
{
	return true;
}

bool IntroScene::CleanUp()
{

	return true;
}