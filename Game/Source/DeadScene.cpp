#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "DeadScene.h"
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
		deadTexture = app->tex->Load("Assets/Textures/Scenes/lose.png");
		app->render->SetCameraPosition(0, 0);

		btnRestartLevel = new GuiButton(1, { 520, 300, 125, 18 }, "  RESTART");
		btnRestartLevel->SetObserver(this);

		btnBackToMenu = new GuiButton(2, { 520, 350, 125, 18 }, "BACK TO MENU");
		btnBackToMenu->SetObserver(this);
	}

	return true;
}

bool DeadScene::Update(float dt)
{
	btnRestartLevel->Update(app->input, dt, iPoint(520, 300));
	btnBackToMenu->Update(app->input, dt, iPoint(520, 350));

	return true;
}

bool DeadScene::Draw()
{
	bool ret = true;

	app->render->DrawTexture(deadTexture, 0, 0, NULL);
	
	btnBackToMenu->Draw(app->render, guiDebugDraw);
	btnRestartLevel->Draw(app->render, guiDebugDraw);

	return ret;
}

bool DeadScene::CleanUp()
{
	app->tex->UnLoad(deadTexture);

	delete btnBackToMenu;
	delete btnRestartLevel;

	return true;
}

bool DeadScene::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control->id == 1)
	{
		TransitionToScene(app->sceneManager->lastScene);
	}
	else if (control->id == 2)
	{
		TransitionToScene((Scene*)app->sceneManager->mainMenu);
	}

	return true;
}
