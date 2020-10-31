#include "App.h"

#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "IntroScene.h"
#include "Window.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "WinScene.h"

WinScene::WinScene()
{

	name.Create("winScene");
}

WinScene::~WinScene()
{
}

bool WinScene::Awake(pugi::xml_node& config)
{

	return true;
}

bool WinScene::Start()
{
	winTexture = app->tex->Load("Assets/textures/WinScene.png");
	app->render->SetCameraPosition(0, 0);

	return true;
}

bool WinScene::PreUpdate()
{
	return true;
}

bool WinScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		app->fade->FadingToBlack(this, app->intro, 30.0f);
	}

	return true;
}

bool WinScene::PostUpdate()
{

	app->render->DrawTexture(winTexture, 0, 0, NULL);

	return true;
}

bool WinScene::CleanUp()
{
	app->tex->UnLoad(winTexture);
	return true;
}
