
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
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"


IntroScene::IntroScene()
{
	name.Create("introScene");

}

IntroScene::~IntroScene()
{

}

bool IntroScene::Awake(pugi::xml_node& node)
{
	return true;
}

// Load assets
bool IntroScene::Start()
{
	intro = app->tex->Load("Assets/textures/intro.png");

	return true;
}

bool IntroScene::Update(float dt)
{
	
	app->fade->FadingToBlack(this, app->scene, 30);
	return true;
}

// Update: draw background
bool IntroScene::PostUpdate()
{

//	app->render->DrawTexture(intro,0,0,NULL);
	return true;
}

bool IntroScene::CleanUp()
{

	return true;
}