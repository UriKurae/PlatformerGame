#include "App.h"

#include "Input.h"
#include "Fonts.h"
#include "SceneManager.h"
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

	score = app->sceneManager->CalculateFinalScore();

	return true;
}

bool WinScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
		TransitionToScene(app->sceneManager->mainMenu);
		//app->fade->Fade(this, app->sceneManager->mainMenu, 1 / dt);

	return true;
}

bool WinScene::Draw()
{
	app->render->DrawTexture(winTexture, 0, 0, NULL);

	DrawFinalScreen();

	return true;
}

bool WinScene::CleanUp()
{
	app->tex->UnLoad(winTexture);
	return true;
}

void WinScene::DrawFinalScreen()
{
	uint x, y;
	app->win->GetWindowSize(x, y);
	int offsetY = app->win->GetScale();
	int offsetX = app->win->GetScale();
	SDL_Rect r = { 240 / offsetX, 340 / offsetY, 780 / offsetX, 230 / offsetY };

	app->render->DrawRectangle(r, { 0,0,0,225 });

	sprintf_s(wolvesText, 4, "%d", app->sceneManager->wolvesKilled);
	app->fonts->DrawText(260 / offsetX, 360 / offsetY, uiIndex, "TOTAL WOLVES KILLED");
	app->fonts->DrawText(970 / offsetX, 360 / offsetY, uiIndex, wolvesText);

	sprintf_s(executionerText, 4, "%d", app->sceneManager->executionersKilled);
	app->fonts->DrawText(260 / offsetX, 400 / offsetY, uiIndex, "TOTAL EXECUTIONERS KILLED");
	app->fonts->DrawText(970 / offsetX, 400 / offsetY, uiIndex, executionerText);

	sprintf_s(gemsText, 4, "%d", app->sceneManager->gemsPicked);
	app->fonts->DrawText(260 / offsetX, 440 / offsetY, uiIndex, "TOTAL GEMS PICKED");
	app->fonts->DrawText(970 / offsetX, 440 / offsetY, uiIndex, gemsText);

	sprintf_s(heartText, 4, "%d", app->sceneManager->heartsPicked);
	app->fonts->DrawText(260 / offsetX, 480 / offsetY, uiIndex, "TOTAL HEARTS PICKED");
	app->fonts->DrawText(970 / offsetX, 480 / offsetY, uiIndex, heartText);

	sprintf_s(finalScoreText, 4, "%d", score);
	app->fonts->DrawText(260 / offsetX, 520 / offsetY, uiIndex, "AMAZING! YOUR FINAL SCORE IS");
	app->fonts->DrawText(970 / offsetX, 520 / offsetY, uiIndex, finalScoreText);

}
