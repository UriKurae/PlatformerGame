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
		
	}
	// L03: DONE: Load map
	app->map->Load("Level1.tmx");
	//bgImage = app->tex->Load("Assets/textures/bg.png");
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	sky = app->tex->Load("Assets/textures/sky2.png");
	sea = app->tex->Load("Assets/textures/sea2.png");
	clouds = app->tex->Load("Assets/textures/clouds2.png");
	playerStartPosition = app->player->SetPosition(230, 128);
	
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
				app->fade->FadingToBlack(this, (Module*)app->winScene, 30.0f);
				app->player->Disable();
			}
			if (playerMidTile == 1170)
			{
				app->fade->FadingToBlack(this, (Module*)app->deadScene, 30.0f);
				app->player->Disable();
			}

		}
			layer = layer->next;

	}
}
	
