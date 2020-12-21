#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "SceneMainMenu.h"
#include "SceneManager.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"


MainMenu::MainMenu()
{
	name.Create("mainMenu");
}

MainMenu::~MainMenu()
{

}

// Load assets
bool MainMenu::Start()
{
	menuState = MenuState::INITIAL;
	//intro = app->tex->Load("Assets/Textures/Scenes/intro.png");
	intro = app->tex->Load("Assets/Textures/Scenes/main_test.png");
	
	app->audio->PlayMusic("Assets/Audio/Music/intro_scene.ogg");

	app->render->SetCameraPosition(0,0);

	btnPlay = new GuiButton(1, { 289, 147, 65, 15 }, "START");
	btnPlay->SetObserver(this);

	btnContinue = new GuiButton(2, { 278, 166, 86, 15 }, "CONTINUE");
	btnContinue->SetObserver(this);

	btnSettings = new GuiButton(3, { 280, 187, 78, 13 }, "SETTINGS");
	btnSettings->SetObserver(this);

	btnCredits = new GuiButton(4, { 282, 208, 77, 13 }, "CREDITS");
	btnCredits->SetObserver(this);

	btnExit = new GuiButton(5, { 298, 226, 44, 14 }, "EXIT");
	btnExit->SetObserver(this);

	return true;
}

bool MainMenu::Update(float dt)
{
	if (menuState == MenuState::INITIAL)
	{
		btnPlay->Update(app->input, dt);
		btnContinue->Update(app->input, dt);
		btnSettings->Update(app->input, dt);
		btnCredits->Update(app->input, dt);
		btnExit->Update(app->input, dt);
	}
	else if (menuState == MenuState::OPTIONS)
	{

	}
	else if (menuState == MenuState::CREDITS)
	{

	}

	return true;
}

// Update: draw background
bool MainMenu::Draw()
{
	bool ret = true;

	app->render->DrawTexture(intro, 0, 0, NULL);

	if (menuState == MenuState::INITIAL)
	{
		btnPlay->Draw(app->render);
		btnContinue->Draw(app->render);
		btnSettings->Draw(app->render);
		btnCredits->Draw(app->render);
		btnExit->Draw(app->render);
	}
	else if (menuState == MenuState::OPTIONS)
	{

	}
	else if (menuState == MenuState::CREDITS)
	{

	}

	return ret;
}

bool MainMenu::CleanUp()
{
	app->tex->UnLoad(intro);

	return true;
}


bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) TransitionToScene((Scene*)app->sceneManager->scene1);
	}
	default: break;
	}

	return true;
}