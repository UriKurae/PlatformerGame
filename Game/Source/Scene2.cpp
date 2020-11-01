#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene2.h"
#include "Map.h"
#include "Player.h"
#include "Scene.h"
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

	folder.Create(config.child("folder").child_value());

	return ret;
}

// Called before the first frame
bool Scene2::Start()
{


	if (this->active == true)
	{
		app->player->Enable();
		app->audio->PlayMusic("Assets/audio/music/JRPG Battle Theme - loop 168bpm.ogg");
		app->map->Load("Level2.tmx");

		sky = app->tex->Load("Assets/textures/sky2.png");
		sea = app->tex->Load("Assets/textures/sea2.png");
		clouds = app->tex->Load("Assets/textures/clouds2.png");
		playerStartPosition = app->player->SetPosition(230, 230);

		app->map->Enable();
		app->scene->Disable();
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
	// L02: TODO 3: Request Load / Save when pressing L/S
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


	CheckWin();

	return true;
}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	app->render->DrawTexture(sky, 0, -10, NULL, 0.65f);
	app->render->DrawTexture(clouds, 0, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, 0, 395, NULL, 0.85f);

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


void Scene2::CheckWin()
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
				app->fade->FadingToBlack(this, (Module*)app->winScene, 500.0f);
				app->player->Disable();
			}
			if (playerMidTile == 1170)
			{
				app->fade->FadingToBlack(this, (Module*)app->deadScene, 500.0f);
				app->player->Disable();
			}

		}
		layer = layer->next;

	}

}