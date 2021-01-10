#include "App.h"

#include "Input.h"
#include "SceneManager.h"
#include "Textures.h"
#include "Render.h"
#include "Fonts.h"
#include "SceneMainMenu.h"
#include "SceneManager.h"
#include "Window.h"
#include "Scene.h"
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
	status = SceneState::INITIAL;

	winTexture = app->tex->Load("Assets/Textures/Scenes/win.png");
	app->render->SetCameraPosition(0, 0);

	score = app->sceneManager->CalculateFinalScore();

	btnSeeScore = new GuiButton(1, { 520,350,125,18 }, " SEE SCORE");
	btnSeeScore->SetObserver(this);

	btnBackToMenu = new GuiButton(2, { 520,400,125,18 }, "BACK TO MENU");
	btnBackToMenu->SetObserver(this);

	btnBackSeeScore = new GuiButton(3, { 900,700,125,18 }, "    BACK");
	btnBackSeeScore->SetObserver(this);

	showScore = false;
	guiDebugDraw = false;

	return true;
}

bool WinScene::Update(float dt)
{
	if (status == SceneState::INITIAL)
	{
		btnSeeScore->Update(app->input, dt, iPoint(520, 350));
		btnBackToMenu->Update(app->input, dt, iPoint(520, 400));
	}
	else if (status == SceneState::SCORE)
	{
		btnBackSeeScore->Update(app->input, dt, iPoint(1010, 670));
	}

	return true;
}

bool WinScene::Draw()
{
	app->render->DrawTexture(winTexture, 0, 0, NULL);

	if (status == SceneState::INITIAL)
	{
		uint x, y;
		app->win->GetWindowSize(x, y);
		int offsetY = app->win->GetScale();
		int offsetX = app->win->GetScale();
		SDL_Rect r = { 468 / offsetX, 294 / offsetY, 350 / offsetX, 200 / offsetY };

		app->render->DrawRectangle(r, { 0,0,0,225 });

		btnSeeScore->Draw(app->render, guiDebugDraw);
		btnBackToMenu->Draw(app->render, guiDebugDraw);
	}
	else if (status == SceneState::SCORE)
	{
		DrawScore();
		btnBackSeeScore->Draw(app->render, guiDebugDraw);
	}
	return true;
}

bool WinScene::CleanUp()
{
	app->tex->UnLoad(winTexture);

	delete btnBackSeeScore;
	delete btnBackToMenu;
	delete btnSeeScore;
	
	return true;
}

bool WinScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// Back to menu
	if (control->id == 1)
		status = SceneState::SCORE;
		
	else if (control->id == 2)
		TransitionToScene(app->sceneManager->mainMenu);
	
	else if (control->id == 3)
		status = SceneState::INITIAL;


	return true;
}

void WinScene::DrawScore()
{
	uint x, y;
	app->win->GetWindowSize(x, y);
	int offsetY = app->win->GetScale();
	int offsetX = app->win->GetScale();
	SDL_Rect r = { 240 / offsetX, 340 / offsetY, 780 / offsetX, 230 / offsetY };

	app->render->DrawRectangle({ 0,0,(int)x,(int)y }, { 0,0,0,120 });
	app->render->DrawRectangle(r, { 0,0,0,225 });

	sprintf_s(wolvesText, 4, "%d", app->sceneManager->wolvesKilled);
	app->fonts->DrawText(260 / offsetX, 360 / offsetY, 0, "TOTAL WOLVES KILLED");
	app->fonts->DrawText(970 / offsetX, 360 / offsetY, 0, wolvesText);

	sprintf_s(executionerText, 4, "%d", app->sceneManager->executionersKilled);
	app->fonts->DrawText(260 / offsetX, 400 / offsetY, 0, "TOTAL EXECUTIONERS KILLED");
	app->fonts->DrawText(970 / offsetX, 400 / offsetY, 0, executionerText);

	sprintf_s(gemsText, 4, "%d", app->sceneManager->gemsPicked);
	app->fonts->DrawText(260 / offsetX, 440 / offsetY, 0, "TOTAL GEMS PICKED");
	app->fonts->DrawText(970 / offsetX, 440 / offsetY, 0, gemsText);

	sprintf_s(heartText, 4, "%d", app->sceneManager->heartsPicked);
	app->fonts->DrawText(260 / offsetX, 480 / offsetY, 0, "TOTAL HEARTS PICKED");
	app->fonts->DrawText(970 / offsetX, 480 / offsetY, 0, heartText);

	sprintf_s(finalScoreText, 4, "%03i", score);
	app->fonts->DrawText(260 / offsetX, 520 / offsetY, 0, "AMAZING! YOUR FINAL SCORE IS");
	app->fonts->DrawText(930 / offsetX, 520 / offsetY, 0, finalScoreText);
}