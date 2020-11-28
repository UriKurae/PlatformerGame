#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "DeadScene.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	if (this->active == true) 
	{
		//app->audio->PlayMusic("Assets/audio/music/JRPG Battle Theme - loop 168bpm.ogg");

		app->map->Load("Level1.tmx");

		sky = app->tex->Load("Assets/textures/sky.png");
		sea = app->tex->Load("Assets/textures/sea.png");
		clouds = app->tex->Load("Assets/textures/clouds.png");

		app->player->Enable();
		app->player->currentLevel = 1;
		playerStartPosition = app->player->SetPosition(250, 70);

		app->map->ResetPath(iPoint(15, 15));

	}
	
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		app->RequestLoadGame();
	
	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->RequestSaveGame();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN)
		RestartPlayerPosition();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KeyState::KEY_DOWN)
		app->map->viewHitboxes = !app->map->viewHitboxes;

	if (app->input->GetKey(SDL_SCANCODE_F2) == KeyState::KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene2, 1/dt);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(-4);
	
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(4);


	if (app->player->godMode == false)
	{
		if (CheckWin() == 1)
		{
			app->fade->FadingToBlack(this, (Module*)app->scene2, 1/dt);
			app->player->Disable();
		}

		else if (CheckWin() == 2)
		{
			app->deadScene->lastScene = this;
			app->fade->FadingToBlack(this, (Module*)app->deadScene, 1/dt);
			RestartPlayerPosition();
			app->player->Disable();
		}
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(sky, -200, -10, NULL, 0.65f);
	app->render->DrawTexture(clouds, -200, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, -200, 395, NULL, 0.85f);

	if(app->map->active == true)
		app->map->Draw();
		

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		app->map->PropagateBFS();
		//app->map->PropagateDijkstra();
		app->map->DrawPath();
	}
	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_UP)
	{
		app->map->ResetPath(iPoint(15, 15));
	}



	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	app->map->Disable();
	
	return true;
}

bool Scene::RestartPlayerPosition()
{
	app->player->SetPosition(playerStartPosition.x, playerStartPosition.y);

	return true;
}


int Scene::CheckWin()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint playerPosTop = app->map->WorldToMap(app->player->GetPosition().x+8, app->player->GetPosition().y+15);

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