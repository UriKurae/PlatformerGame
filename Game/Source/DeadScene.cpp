#include "App.h"

#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "IntroScene.h"
#include "Window.h"
#include "DeadScene.h"
#include "FadeToBlack.h"
#include "WinScene.h"

DeadScene::DeadScene()
{

	name.Create("DeadScene");
}

DeadScene::~DeadScene()
{
}

bool DeadScene::Awake(pugi::xml_node& config)
{

	return true;
}

bool DeadScene::Start()
{
	if (this->active == true)
	{
		deadTexture = app->tex->Load("Assets/textures/LoseScene.png");
		app->render->SetCameraPosition(0, 0);
	}

	return true;
}

bool DeadScene::PreUpdate()
{
	return true;
}

bool DeadScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		app->fade->FadingToBlack(this, app->intro, 30.0f);
	}

	return true;
}

bool DeadScene::PostUpdate()
{

	app->render->DrawTexture(deadTexture, 0, 0, NULL);

	return true;
}

bool DeadScene::CleanUp()
{
	app->tex->UnLoad(deadTexture);
	return true;
}
