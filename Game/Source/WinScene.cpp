#include "App.h"

#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "SceneMainMenu.h"
#include "SceneManager.h"
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

bool WinScene::Start()
{
	winTexture = app->tex->Load("Assets/Textures/Scenes/win.png");
	app->render->SetCameraPosition(0, 0);

	return true;
}

bool WinScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
		app->fade->Fade(this, app->sceneManager->mainMenu, 1 / dt);

	return true;
}

bool WinScene::Draw()
{
	app->render->DrawTexture(winTexture, 0, 0, NULL);

	return true;
}

bool WinScene::CleanUp()
{
	app->tex->UnLoad(winTexture);
	return true;
}
