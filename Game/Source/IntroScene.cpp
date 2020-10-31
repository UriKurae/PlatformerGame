
#include "App.h"
#include "IntroScene.h"

#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"


IntroScene::IntroScene()
{
	name.Create("introScene");
	count = 10000;
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
	logo = app->tex->Load("Assets/textures/logo.png");
	app->render->SetCameraPosition(0,0);

	return true;
}

bool IntroScene::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN && count == 0) 
	{ 
		app->fade->FadingToBlack(this, app->scene, 60.0f);
	}
	
	return true;
}

// Update: draw background
bool IntroScene::PostUpdate()
{
	count -= 0.5f;
	if (count <= 10000 && count > 0)
	{
		app->render->DrawTexture(logo, 150, 0, NULL);
	}
	else
	{
		app->render->DrawTexture(intro, 0, 0, NULL);
		count = 0;
	}
	
	return true;
}

bool IntroScene::CleanUp()
{

	return true;
}