#include "App.h"

#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "DeadScene.h"
#include "FadeToBlack.h"
#include "SceneManager.h"

DeadScene::DeadScene()
{

	name.Create("DeadScene");
}

DeadScene::~DeadScene()
{
}

bool DeadScene::Start()
{
	if (this->active == true)
	{
		deadTexture = app->tex->Load("Assets/Textures/lose.png");
		app->render->SetCameraPosition(0, 0);
	}

	return true;
}

bool DeadScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		if (app->sceneManager->lastScene == (Scene*)app->sceneManager->scene1)
			app->fade->Fade(this, (Scene*)app->sceneManager->lastScene, 1 / dt);

		else if (app->sceneManager->lastScene == (Scene*)app->sceneManager->scene2)
			app->fade->Fade(this, (Scene*)app->sceneManager->lastScene, 1 / dt);
	}

	return true;
}

bool DeadScene::Draw()
{
	bool ret = true;

	app->render->DrawTexture(deadTexture, 0, 0, NULL);

	return ret;
}

bool DeadScene::CleanUp()
{
	app->tex->UnLoad(deadTexture);

	return true;
}
