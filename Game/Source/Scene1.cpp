#include "App.h"
#include "Audio.h"
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
#include "ItemManager.h"
#include "GreenGem.h"
#include "Wolf.h"
#include "FadeToBlack.h"
#include "Collisions.h"

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
		app->map->Load("level_1.tmx");

		if (app->player->active == false)
			app->player->Enable();

		if ((app->player->loadedGame) && (app->sceneManager->savedScene == this))
			app->player->SetPosition(app->player->savedPosition.x, app->player->savedPosition.y);

		else playerStartPosition = app->player->SetPosition(250, 5);

		// Enemy instantiation
		executioner = (Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(400, 100));
		executioner->Start();

		wolf = (Wolf*)app->enemyManager->AddEnemy(EnemyType::GROUND, iPoint(350, 250));
		wolf->Start();

		wolf2 = (Wolf*)app->enemyManager->AddEnemy(EnemyType::GROUND, iPoint(450, 250));
		wolf2->Start();

		executioner2 = (Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(600, 100));
		executioner2->Start();

		// Items instantiation
		gem = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(1200, 140));
		gem->Start();

		// Assets loading and playing
		app->audio->PlayMusic("Assets/Audio/Music/scene_1.ogg");
		sky = app->tex->Load("Assets/Textures/sky.png");
		sea = app->tex->Load("Assets/Textures/sea.png");
		clouds = app->tex->Load("Assets/Textures/clouds.png");
	

		app->sceneManager->currentScene = this;

	}
	return true;
}

bool Scene1::Update(float dt)
{	
	if (CheckWin() == 1)
	{
		app->fade->Fade(this, (Scene*)app->sceneManager->scene2, 1/dt);
	}

	else if (CheckWin() == 2)
	{
		app->fade->Fade(this, (Scene*)app->sceneManager->deadScene, 1 / dt);
		app->sceneManager->lastScene = this;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		RestartPlayerPosition();
	}

	app->sceneManager->checkPointKeepAnim.speed = 8.0f * dt;
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
	
	app->player->Draw();
	app->enemyManager->Draw();
	gem->Draw();

	if (checkPoint1 == true)
	{

		if (currentAnim != &app->sceneManager->checkPointKeepAnim)
		{
			app->sceneManager->checkPointKeepAnim.Reset();
			currentAnim = &app->sceneManager->checkPointKeepAnim;
		}
		app->render->DrawTexture(app->sceneManager->checkPointTexture, 1535, 155, &currentAnim->GetCurrentFrame());
		currentAnim->Update();

	}
	else if (checkPoint2 == true)
	{
		if (currentAnim != &app->sceneManager->checkPointKeepAnim)
		{
			app->sceneManager->checkPointKeepAnim.Reset();
			currentAnim = &app->sceneManager->checkPointKeepAnim;
		}
		app->render->DrawTexture(app->sceneManager->checkPointTexture, 2256, 268, &currentAnim->GetCurrentFrame());
		currentAnim->Update();
	}


	return ret;
}

bool Scene1::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	app->map->CleanUp();

	app->player->Disable();

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
				currentAnim = &app->sceneManager->checkPointKeepAnim;
				if (checkSound1 == false)
				{
					app->audio->PlayFx(app->sceneManager->checkFx);
					checkSound1 = true;
					checkSound2 = false;
				}
			}
			else if (playerMidTile == 1168)
			{
 				checkPoint2 = true;
				checkPoint1 = false;
				if (checkSound2 == false)
				{
					app->audio->PlayFx(app->sceneManager->checkFx);
					checkSound1 = false;
					checkSound2 = true;
				}
			}

		}
	
		layer = layer->next;
	}
	
	return -1;
}
