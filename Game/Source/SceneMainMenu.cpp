#include "App.h"
#include "Window.h"
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

	// Initial buttons
	btnPlay = new GuiButton(1, { 575 / (int)app->win->GetScale(), 289 / (int)app->win->GetScale(), 65, 15 }, "START");
	btnPlay->SetObserver(this);

	btnContinue = new GuiButton(2, { 575 / (int)app->win->GetScale(), 331 / (int)app->win->GetScale(), 65, 15 }, "CONTINUE");
	btnContinue->SetObserver(this);

	btnSettings = new GuiButton(3, { 575 / (int)app->win->GetScale(), 372 / (int)app->win->GetScale(), 65, 15 }, "SETTINGS");
	btnSettings->SetObserver(this);

	btnCredits = new GuiButton(4, { 575 / (int)app->win->GetScale(), 414 / (int)app->win->GetScale(), 65, 15 }, "CREDITS");
	btnCredits->SetObserver(this);

	btnExit = new GuiButton(5, { 575 / (int)app->win->GetScale(), 455 / (int)app->win->GetScale(), 65, 15 }, "EXIT");
	btnExit->SetObserver(this);


	// Options menu
	sliderMusicVolume = new GuiSlider(1, { 320, 150, 5, 10 }, "SLIDERMUSIC");
	sliderMusicVolume->SetObserver(this);

	sliderFxVolume = new GuiSlider(2, {320, 170, 5, 10}, "SLIDERFX");
	sliderFxVolume->SetObserver(this);

	fullScreenCheckBox = new GuiCheckBox(1, {359, 190, 10, 10}, "FULLSCREEN");
	fullScreenCheckBox->SetObserver(this);

	vSyncCheckBox = new GuiCheckBox(2, {359,210,10,10}, "VSYNC");
	vSyncCheckBox->SetObserver(this);

	btnBackOptions = new GuiButton(6, { 300, 226, 65, 15 }, "BACKOPTIONS");
	btnBackOptions->SetObserver(this);

	// Credits menu
	btnBackCredits = new GuiButton(7, {602,332,50,15}, "BACKCREDITS");
	btnBackCredits->SetObserver(this);

	return true;
}

bool MainMenu::Update(float dt)
{
	if (menuState == MenuState::INITIAL)
	{
		btnPlay->Update(app->input, dt, iPoint(575 / app->win->GetScale(), 289 / app->win->GetScale()));
		btnContinue->Update(app->input, dt, iPoint(575 / app->win->GetScale(), 331 / app->win->GetScale()));
		btnSettings->Update(app->input, dt, iPoint(575 / app->win->GetScale(), 372 / app->win->GetScale()));
		btnCredits->Update(app->input, dt, iPoint(575 / app->win->GetScale(),  414 / app->win->GetScale()));
		btnExit->Update(app->input, dt, iPoint(575 / app->win->GetScale(), 455 / app->win->GetScale()));
	}
	else if (menuState == MenuState::OPTIONS)
	{
		sliderMusicVolume->Update(app->input, dt, iPoint(635 / app->win->GetScale(), 289 / app->win->GetScale()));
		sliderFxVolume->Update(app->input, dt, iPoint(635 / app->win->GetScale(), 331 / app->win->GetScale()));
		fullScreenCheckBox->Update(app->input, dt, iPoint(735 / app->win->GetScale(), 372 / app->win->GetScale()));
		vSyncCheckBox->Update(app->input, dt, iPoint(735 / app->win->GetScale(), 414 / app->win->GetScale()));
		btnBackOptions->Update(app->input, dt, iPoint(575 / app->win->GetScale(), 455 / app->win->GetScale()));

		app->audio->SetMusicVolume(sliderMusicVolume->GetValue());
		app->audio->SetFXVolume(sliderFxVolume->GetValue());
		
	}
	else if (menuState == MenuState::CREDITS)
	{
		btnBackCredits->Update(app->input, dt, iPoint(app->render->camera.x + 500, app->render->camera.y + 250));
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
		sliderMusicVolume->Draw(app->render);
		sliderFxVolume->Draw(app->render);
		fullScreenCheckBox->Draw(app->render);
		vSyncCheckBox->Draw(app->render);
		btnBackOptions->Draw(app->render);
	}
	else if (menuState == MenuState::CREDITS)
	{
		uint x, y;
		app->win->GetWindowSize(x, y);
		SDL_Rect r = { -(app->render->camera.x + 500),-(app->render->camera.y + 250),x,y };

		app->render->DrawRectangle(r, { 0,0,0,225 });
		btnBackCredits->Draw(app->render);
	}

	return ret;
}

bool MainMenu::CleanUp()
{
	app->tex->UnLoad(intro);
	delete btnPlay;
	delete btnContinue;
	delete btnSettings;
	delete btnBackCredits;
	delete btnExit;
	delete sliderFxVolume;
	delete sliderMusicVolume;
	delete fullScreenCheckBox;
	delete vSyncCheckBox;
	delete btnBackOptions;

	return true;
}


bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) TransitionToScene((Scene*)app->sceneManager->scene1);
		else if (control->id == 3) menuState = MenuState::OPTIONS;
		else if (control->id == 4) menuState = MenuState::CREDITS;
		else if (control->id == 5) toExit = true;
		else if (control->id == 6) menuState = MenuState::INITIAL;
		else if (control->id == 7) menuState = MenuState::INITIAL;
	}
	default: break;
	}

	return true;
}