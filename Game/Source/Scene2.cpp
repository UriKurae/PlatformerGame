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
	if (this->active)
	{
		app->map->active = true;
		app->map->Load("level_2.tmx");

		app->player->Enable();
		app->player->SetPosition(250, 20);

		sky = app->tex->Load("Assets/Textures/sky.png");
		sea = app->tex->Load("Assets/Textures/sea.png");
		clouds = app->tex->Load("Assets/Textures/clouds.png");

		app->player->Enable();

		if ((app->player->loadedGame) && (app->sceneManager->savedScene == this))
		{
			app->player->SetPosition(app->player->savedPosition.x, app->player->savedPosition.y);
		}
		else
		{
			playerStartPosition = app->player->SetPosition(250, 5);
		}
		app->sceneManager->currentScene = this;
	}

	return true;
	
}

bool Scene2::Update(float dt)
{
	if (CheckWin() == 1)
	{
		app->fade->Fade(this, (Scene*)app->sceneManager->scene2, 1 / dt);
	}
	else if (CheckWin() == 2)
	{
		deadOnScene = true;
		app->fade->Fade(this, (Scene*)app->sceneManager->deadScene, 1 / dt);
		app->sceneManager->lastScene = this;
		app->player->Disable();
	}


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		RestartPlayerPosition();
	}

	return true;
}

bool Scene2::Draw()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(sky, -200, -10, NULL, 0.65f);
	app->render->DrawTexture(clouds, -200, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, -200, 395, NULL, 0.85f);

	if (app->map->active)
		app->map->Draw();

	if(app->player->active)
		app->player->Draw();
	
	return ret;
}

bool Scene2::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	app->map->CleanUp();
	app->player->Disable();


	return true;
}

bool Scene2::RestartPlayerPosition()
{
	if (checkPoint1 == true)
	{
		app->player->SetPosition(1552, 464);
	}
	else if (checkPoint2 == true)
	{
		app->player->SetPosition(3024, 208);
	}
	else
	{
		app->player->SetPosition(250, 70);
	}

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

			if (playerMidTile == 1167 && checkPoint2 == false)
			{
				checkPoint1 = true;
			}
			else if (playerMidTile == 1168)
			{
				checkPoint2 = true;
				checkPoint1 = false;
			}
		}

		layer = layer->next;
	}

	return -1;
}
