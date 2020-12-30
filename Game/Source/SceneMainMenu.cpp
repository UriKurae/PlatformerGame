#include "App.h"
#include "Audio.h"
#include "Window.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "SceneMainMenu.h"
#include "SceneManager.h"
#include "Fonts.h"

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
	app->render->SetCameraPosition(0,0);
	
	intro = app->tex->Load("Assets/Textures/Scenes/main_test.png");
	textureCredits = app->tex->Load("Assets/Textures/credits.png");

	app->audio->PlayMusic("Assets/Audio/Music/intro_scene.ogg");

	char lookupTable[] = { "!,-.0123456789?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz " };
	uiIndex = app->fonts->Load("Assets/Textures/fonts.png", lookupTable, 1);

	// Initial buttons
	btnPlay = new GuiButton(1, { 520, 250, 125, 18 }, "   START");
	btnPlay->SetObserver(this);

	btnContinue = new GuiButton(2, { 520, 300, 125, 18 }, "  CONTINUE");
	btnContinue->SetObserver(this);

	btnSettings = new GuiButton(3, { 520, 350, 125, 18 }, "  SETTINGS");
	btnSettings->SetObserver(this);

	btnCredits = new GuiButton(4, { 520, 400, 125, 18 }, "   CREDITS");
	btnCredits->SetObserver(this);

	btnExit = new GuiButton(5, { 520, 500, 125, 18 }, "    EXIT");
	btnExit->SetObserver(this);


	// Options menu
	sliderMusicVolume = new GuiSlider(1, { 555, 280, 5, 15 }, " MUSIC VOLUME");
	sliderMusicVolume->SetObserver(this);

	sliderFxVolume = new GuiSlider(2, {555, 360, 5, 15}, "   FX VOLUME");
	sliderFxVolume->SetObserver(this);

	fullScreenCheckBox = new GuiCheckBox(1, {735, 410, 16, 16}, "FULLSCREEN");
	fullScreenCheckBox->SetObserver(this);

	vSyncCheckBox = new GuiCheckBox(2, {735,460,16,16}, "VSYNC");
	vSyncCheckBox->SetObserver(this);

	btnBackOptions = new GuiButton(6, { 520, 560, 125, 18 }, "    BACK");
	btnBackOptions->SetObserver(this);

	// Credits menu
	btnBackCredits = new GuiButton(7, {602,332,125,18}, "    BACK");
	btnBackCredits->SetObserver(this);


	guiDebugDraw = false;

	return true;
}

bool MainMenu::Update(float dt)
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	if (menuState == MenuState::INITIAL)
	{
		btnPlay->Update(app->input, dt, iPoint(520, 250));
		btnContinue->Update(app->input, dt, iPoint(520, 300));
		if (app->sceneManager->saveAvailable == false)
		{
			btnContinue->state = GuiControlState::DISABLED;
		}
		else
		{
			btnSettings->Update(app->input, dt, iPoint(520, 350));
		}
		btnCredits->Update(app->input, dt, iPoint(520, 400 ));
		btnExit->Update(app->input, dt, iPoint(520, 500));
	}
	else if (menuState == MenuState::OPTIONS)
	{
		sliderMusicVolume->Update(app->input, dt, iPoint(585, 250));
		sliderFxVolume->Update(app->input, dt, iPoint(585, 330));
		fullScreenCheckBox->Update(app->input, dt, iPoint(735, 380));
		vSyncCheckBox->Update(app->input, dt, iPoint(735, 430));
		btnBackOptions->Update(app->input, dt, iPoint(520, 530));

		app->audio->SetMusicVolume(sliderMusicVolume->GetValue());
		app->audio->SetFXVolume(sliderFxVolume->GetValue());
		
	}
	else if (menuState == MenuState::CREDITS)
	{
		btnBackCredits->Update(app->input, dt, iPoint((app->render->camera.x + 1012), (app->render->camera.y + 665)));
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
		app->render->DrawRectangle({ 450 / (int)app->win->GetScale(), 190 / (int)app->win->GetScale(), 200, 200 }, { 0,0,0,200 });
		btnPlay->Draw(app->render, guiDebugDraw);
		btnContinue->Draw(app->render, guiDebugDraw);
		btnSettings->Draw(app->render, guiDebugDraw);
		btnCredits->Draw(app->render, guiDebugDraw);
		btnExit->Draw(app->render, guiDebugDraw);
	}
	else if (menuState == MenuState::OPTIONS)
	{
		app->render->DrawRectangle({ 450 / (int)app->win->GetScale(), 190 / (int)app->win->GetScale(), 200, 200 }, { 0,0,0,200 });
		sliderMusicVolume->Draw(app->render, guiDebugDraw);
		sliderFxVolume->Draw(app->render, guiDebugDraw);
		fullScreenCheckBox->Draw(app->render, guiDebugDraw);
		vSyncCheckBox->Draw(app->render, guiDebugDraw);
		btnBackOptions->Draw(app->render, guiDebugDraw);
	}
	else if (menuState == MenuState::CREDITS)
	{
		uint x, y;
		app->win->GetWindowSize(x, y);
		SDL_Rect r = { -(app->render->camera.x + 500),-(app->render->camera.y + 250),x,y };

		app->render->DrawRectangle(r, { 0,0,0,225 });
		app->render->DrawTexture(textureCredits, 0, 0, NULL);

		btnBackCredits->Draw(app->render, guiDebugDraw);
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
		if (control->id == 1) // Play
		{
			app->sceneManager->newGame = true;
			TransitionToScene((Scene*)app->sceneManager->scene1);
		}
		else if (control->id == 2) // Continue
		{
			app->sceneManager->newGame = false;
			Load();
			TransitionToScene((Scene*)app->sceneManager->savedScene);
		}
		else if (control->id == 3) menuState = MenuState::OPTIONS; // Options
		else if (control->id == 4) menuState = MenuState::CREDITS; // Credits
		else if (control->id == 5) toExit = true; // Exit
		else if (control->id == 6) menuState = MenuState::INITIAL; // Back from options
		else if (control->id == 7) menuState = MenuState::INITIAL; // Back from credits
		break;
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 1) app->audio->SetMusicVolume(sliderMusicVolume->GetValue()); // Music
		else if (control->id == 2) app->audio->SetMusicVolume(sliderFxVolume->GetValue()); // FX
		break;
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 1) app->win->fullscreenWindow = !app->win->fullscreenWindow; // FullScreen
		else if (control->id == 2) app->render->vsync = !app->render->vsync; // Vsync
		break;
	}
	default: break;
	}

	return true;
}

void MainMenu::SetContinueButton(GuiControlState state)
{
	if(btnContinue != nullptr)
		btnContinue->state = state;
}

bool MainMenu::Load()
{
	pugi::xml_document tmp;
	
	
	pugi::xml_parse_result resul = tmp.load_file("save_game.xml");
	if (resul != NULL)
	{
		pugi::xml_node node = tmp.child("save_status");

		int saved = node.child("scenemanager").child("active_scene").attribute("value").as_int();

		if (saved == 1)
		{
			app->sceneManager->savedScene = (Scene*)app->sceneManager->scene1;
		}
		else if (saved == 2)
		{
			app->sceneManager->savedScene = (Scene*)app->sceneManager->scene2;
		}

	}


	/*int activeScene = node.child("active_scene").attribute("value").as_int();

	if (activeScene == 1)
		app->sceneManager->savedScene = (Scene*)app->sceneManager->scene1;
	else if (activeScene == 2)
		app->sceneManager->savedScene = (Scene*)app->sceneManager->scene2;*/


	return true;
}
