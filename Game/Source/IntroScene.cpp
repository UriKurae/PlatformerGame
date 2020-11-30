
#include "App.h"
#include "IntroScene.h"

#include "Audio.h"
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
	count = 200;
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
	
	if (this->active == true)
	{
		app->audio->PlayMusic("Assets/audio/music/track_1.ogg");
	}

	app->render->SetCameraPosition(0,0);

	return true;
}

bool IntroScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(-4);

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(4);
	
	if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) && (count <= 0) && (showLogo == false))
		app->fade->FadingToBlack(this, app->scene, 1 / dt);
	
	if (count == 100)
	{
		app->fade->FadingToBlack(this, this, 1 / dt);
	}
	else if (count <= 40)
	{
		showLogo = false;
	}
	
	count -= 20 * dt;

	return true;
}

// Update: draw background
bool IntroScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if ((count >= 65) && (showLogo == true))
	{
		app->render->DrawTexture(logo, 150, 0, NULL);
	}
	else if (count <= 60)
	{
		app->render->DrawTexture(intro, 0, 0, NULL);
	}
	return ret;
}

bool IntroScene::CleanUp()
{
	
	app->tex->UnLoad(intro);
	app->tex->UnLoad(logo);
	
	return true;
}