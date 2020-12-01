#include "App.h"
#include "Log.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Scene1.h"
#include "SceneManager.h"
#include "Scene2.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Executioner.h"
#include "Wolf.h"
#include "FadeToBlack.h"

Scene1::Scene1()
{
	name.Create("scene1");
}

Scene1::~Scene1()
{
}

bool Scene1::Start()
{	
	if (this->active == true)
	{
		app->map->active = true;
		app->map->Load("Level1.tmx");

		app->player->SetPosition(250, 70);

		executioner = (Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(400, 100));
		executioner->Start();

		wolf = (Wolf*)app->enemyManager->AddEnemy(EnemyType::GROUND, iPoint(350, 250));
		wolf->Start();

		sky = app->tex->Load("Assets/textures/sky.png");
		sea = app->tex->Load("Assets/textures/sea.png");
		clouds = app->tex->Load("Assets/textures/clouds.png");
	}
	return true;
}

bool Scene1::Update(float dt)
{	
	if (CheckWin() == 1)
	{
		app->fade->Fade(this, (Scene*)app->sceneManager->scene2, 1/dt);
		app->player->Disable();
	}

	//player->Update(dt);

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		RestartPlayerPosition();
	}

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		executioner->FindTarget(app->player);
	}
	

	return true;
}

bool Scene1::Draw()
{
	bool ret = true;
		
	app->render->DrawTexture(sky, -200, -10, NULL, 0.65f);
	app->render->DrawTexture(clouds, -200, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, -200, 395, NULL, 0.85f);
	
	if(app->map->active == true)
		app->map->Draw();
	
	executioner->Draw();
	wolf->Draw();
	app->player->Draw();

	return ret;
}

bool Scene1::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	app->map->CleanUp();

	wolf->CleanUp();
	RELEASE(wolf);

	executioner->CleanUp();
	RELEASE(executioner);

	return true;
}

bool Scene1::RestartPlayerPosition()
{
	if (checkPoint1 == true)
	{
		app->player->SetPosition(1535, 176);
	}
	else if (checkPoint2 == true)
	{
		app->player->SetPosition(2256, 272);
	}
	else
	{
		app->player->SetPosition(250, 70);
	}

	return true;
}

int Scene1::CheckWin()
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
