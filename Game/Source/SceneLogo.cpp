#include "Textures.h"
#include "SceneLogo.h"
#include "App.h"
#include "Window.h"
#include "Render.h"
#include "SceneManager.h"

SceneLogo::SceneLogo()
{
	name.Create("SceneLogo");
}

SceneLogo::~SceneLogo()
{
}

bool SceneLogo::Start()
{
	counter = 10.0f;
	
	logo = app->tex->Load("Assets/Textures/Scenes/logo.png");

	return true;
}

bool SceneLogo::Update(float dt)
{
	if (counter > 0.0f)
		counter -= 2.0f * dt;
	else
		TransitionToScene((Scene*)app->sceneManager->mainMenu);

	return true;
}

bool SceneLogo::Draw()
{
	uint x, y;
	app->win->GetWindowSize(x, y);
	app->render->DrawTexture(logo, x/4 - 160, y / 4 - 160);

	return true;
}

bool SceneLogo::CleanUp()
{
	app->tex->UnLoad(logo);

	return true;
}
