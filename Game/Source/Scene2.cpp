#include "App.h"
#include "Log.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Scene2.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "SceneManager.h"

Scene2::Scene2()
{
	name.Create("scene2");
}

Scene2::~Scene2()
{
}

bool Scene2::Start()
{
	if (this->active == true)
	{
		app->map->active = true;
		app->map->Load("Level2.tmx");

		//player = new Player(iPoint(250, 70));
		//player->Start();
		app->player->Enable();
		app->player->SetPosition(250,20);

		sky = app->tex->Load("Assets/textures/sky.png");
		sea = app->tex->Load("Assets/textures/sea.png");
		clouds = app->tex->Load("Assets/textures/clouds.png");
	}

	//playerStartPosition() 
	return true;
	
}

bool Scene2::Update(float dt)
{
	if (CheckWin() == 1)
	{
		//app->fade->Fade(this, (Scene*)app->sceneManager->scene2, 1 / dt);
	}

	//player->Update(dt);

	return true;
}

bool Scene2::Draw()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(sky, -200, -10, NULL, 0.65f);
	app->render->DrawTexture(clouds, -200, 180, NULL, 0.75f);
	//app->render->DrawTexture(sea, -200, 395, NULL, 0.85f);

	app->map->Draw();
	player->Draw();


	return ret;
}

bool Scene2::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	RELEASE(player);

	return true;
}

bool Scene2::RestartPlayerPosition()
{
	//app->player->SetPosition(playerStartPosition.x, playerStartPosition.y);

	return true;
}

int Scene2::CheckWin()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	iPoint playerPosTop = app->map->WorldToMap(player->GetPosition().x + 8, player->GetPosition().y + 15);

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
