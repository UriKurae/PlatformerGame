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

	playerKneelDown.PushBack({ 64, 345, 22, 24 });
	playerKneelDown.PushBack({ 115, 345, 22, 24 });
	playerKneelDown.PushBack({ 166, 345, 22, 24 });
	playerKneelDown.loop = false;

	playerOnKnees.PushBack({ 217, 345, 22, 24 });
	playerOnKnees.PushBack({ 266, 345, 22, 24 });
	playerOnKnees.loop = true;
}

DeadScene::~DeadScene()
{
}

bool DeadScene::Start()
{
	if (this->active == true)
	{
		deadTexture = app->tex->Load("Assets/Textures/Scenes/lose.png");
		playerTexture = app->tex->Load("Assets/Textures/Player/player.png");
		app->render->SetCameraPosition(0, 0);

		btnRestartLevel = new GuiButton(1, { 520, 300, 125, 18 }, "  RESTART");
		btnRestartLevel->SetObserver(this);

		btnBackToMenu = new GuiButton(2, { 520, 350, 125, 18 }, "BACK TO MENU");
		btnBackToMenu->SetObserver(this);

		currentPlayerAnim = &playerKneelDown;
		guiDebugDraw = false;
	}

	return true;
}

bool DeadScene::Update(float dt)
{
	playerKneelDown.speed = 2.0f * dt;
	playerOnKnees.speed = 1.0f * dt;

	if (playerKneelDown.HasFinished())
	{
		if (currentPlayerAnim != &playerOnKnees)
		{
			playerOnKnees.Reset();
			currentPlayerAnim = &playerOnKnees;
		}
	}

	btnRestartLevel->Update(app->input, dt, iPoint(520, 300));
	btnBackToMenu->Update(app->input, dt, iPoint(520, 350));

	currentPlayerAnim->Update();

	return true;
}

bool DeadScene::Draw()
{
	bool ret = true;

	app->render->DrawTexture(deadTexture, 0, 0, NULL);
	
	btnBackToMenu->Draw(app->render, guiDebugDraw);
	btnRestartLevel->Draw(app->render, guiDebugDraw);

	if (app->sceneManager->fadeOutCompleted == false)
		app->render->DrawTexture(playerTexture, 300, 255, &currentPlayerAnim->GetCurrentFrame());

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
