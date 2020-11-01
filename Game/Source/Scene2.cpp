#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Scene2.h"
#include "DeadScene.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene2::Scene2() : Module()
{
	name.Create("scene2");
}

// Destructor
Scene2::~Scene2()
{}

// Called before render is available
bool Scene2::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene 2");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene2::Start()
{
	if (this->active == true)
	{
		app->player->Enable();
		

		app->audio->PlayMusic("Assets/audio/music/JRPG Battle Theme - loop 168bpm.ogg");
		app->map->Enable();
		app->map->Load("Level2.tmx");

		sky = app->tex->Load("Assets/textures/sky.png");
		sea = app->tex->Load("Assets/textures/sea.png");
		clouds = app->tex->Load("Assets/textures/clouds.png");
	

		if ((app->player->loadedGame == true) && (app->player->currentLevel == 2) && (deadOnScene == false))
		{
			playerStartPosition = app->player->SetPosition(app->player->GetPosition().x, app->player->GetPosition().y);
		}
		else
		{
			playerStartPosition = app->player->SetPosition(230, 230);
		}

		app->player->currentLevel = 2;
	}



	return true;
}

// Called each loop iteration
bool Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene2::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->RequestLoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->RequestSaveGame();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		RestartLevel();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->map->viewHitboxes = !app->map->viewHitboxes;

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(-4);

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(4);

	if (app->player->godMode == false)
	{
		if (CheckWin() == 1)
		{
			app->fade->FadingToBlack(this, (Module*)app->winScene, 500.0f);
			app->player->Disable();
		}

		else if (CheckWin() == 2)
		{
			app->deadScene->lastScene = this;
			app->fade->FadingToBlack(this, (Module*)app->deadScene, 500.0f);
			app->player->Disable();
			deadOnScene = true;
		}
	}

	return true;
}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(sky, -200, -10, NULL, 0.65f);
	app->render->DrawTexture(clouds, -200, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, -200, 395, NULL, 0.85f);

	if (app->map->active == true)
		app->map->Draw();


	return ret;
}

// Called before quitting
bool Scene2::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	app->map->Disable();

	return true;
}

bool Scene2::RestartLevel()
{
	app->player->SetPosition(playerStartPosition.x, playerStartPosition.y);

	return true;
}


int Scene2::CheckWin()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint playerPosTop = app->map->WorldToMap(app->player->GetPosition().x + 8, app->player->GetPosition().y + 15);

	while (layer != NULL)
	{

		if (layer->data->name == "HitBoxes")
		{
			uint playerMidTile = layer->data->Get(playerPosTop.x, playerPosTop.y);

			if (playerMidTile == 1166)
			{
				return 1;
			}
			if (playerMidTile == 1170)
			{
				return 2;
			}

		}
		layer = layer->next;

	}
	return -1;
}