#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Scene.h"
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

	folder.Create(config.child("folder").child_value());

	return ret;
}

// Called before the first frame
bool Scene::Start()
{


	if (this->active == true) 
	{
		app->player->Enable();
		app->audio->PlayMusic("Assets/audio/music/JRPG Battle Theme - loop 168bpm.ogg");
		app->map->Load("Level1.tmx");

		sky = app->tex->Load("Assets/textures/sky2.png");
		sea = app->tex->Load("Assets/textures/sea2.png");
		clouds = app->tex->Load("Assets/textures/clouds2.png");
		playerStartPosition = app->player->SetPosition(230, 230);

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
	// L02: TODO 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->RequestLoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->RequestSaveGame();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		app->audio->VolumeControl();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		RestartLevel();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		
		app->map->viewHitboxes = !app->map->viewHitboxes;
		
		//ShowColliders();
	}


	CheckWin();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	app->render->DrawTexture(sky, 0, 0, NULL, 0.65f);
	app->render->DrawTexture(clouds, 0, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, 0, 395, NULL, 0.85f);


	app->map->Draw();


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

bool Scene::RestartLevel()
{
	app->player->SetPosition(playerStartPosition.x, playerStartPosition.y);

	return true;
}


void Scene::CheckWin()
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
				app->fade->FadingToBlack(this, (Module*)app->winScene, 60.0f);
				app->player->Disable();
			}
			if (playerMidTile == 1170)
			{
				app->fade->FadingToBlack(this, (Module*)app->deadScene, 60.0f);
				app->player->Disable();
			}

		}
			layer = layer->next;

	}
	
}
bool Scene::ShowColliders()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	ListItem<Properties::Property*>* property;
	while (layer != NULL)
	{
		if (layer->data->name == "HitBoxes")
		{
			property = layer->data->properties.list.start;
			while (property != NULL)
			{
				if (property->data->value == 1 && property->data->name == "Nodraw")
				{
					property->data->value = 0;
				}
				else if(property->data->value == 0 && property->data->name == "Nodraw")
				{
					property->data->value = 1;
				}
				property = property->next;
			}
			break;
		}
		layer = layer->next;
	}

	
	return true;
}
