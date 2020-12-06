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
#include "RedHeart.h"
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

		executioner2 = (Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(600, 100));
		executioner2->Start();

		executioner3 = (Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(2096, 180));
		executioner3->Start();

		wolf = (Wolf*)app->enemyManager->AddEnemy(EnemyType::WOLF, iPoint(400, 250));
		wolf->Start();

		wolf2 = (Wolf*)app->enemyManager->AddEnemy(EnemyType::WOLF, iPoint(650, 260));
		wolf2->Start();

		wolf3 = (Wolf*)app->enemyManager->AddEnemy(EnemyType::WOLF, iPoint(2496, 260));
		wolf3->Start();

		wolf4 = (Wolf*)app->enemyManager->AddEnemy(EnemyType::WOLF, iPoint(3552, 420));
		wolf4->Start();


		// Items instantiation
		gem1 = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(1200, 140));
		gem1->Start();

		gem2 = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(1642, 96));
		gem2->Start();

		gem3 = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(2144, 512));
		gem3->Start();

		gem4 = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(2976, 544));
		gem4->Start();

		heart1 = (RedHeart*)app->itemManager->AddItem(ItemType::HEART, iPoint(432, 176));
		heart1->Start();

		heart2 = (RedHeart*)app->itemManager->AddItem(ItemType::HEART, iPoint(2080, 224));
		heart2->Start();

		heart3 = (RedHeart*)app->itemManager->AddItem(ItemType::HEART, iPoint(2960, 304));
		heart3->Start();


		// Assets loading and playing
		app->audio->PlayMusic("Assets/Audio/Music/scene_1.ogg");
		sky = app->tex->Load("Assets/Textures/Scenes/sky.png");
		sea = app->tex->Load("Assets/Textures/Scenes/sea.png");
		clouds = app->tex->Load("Assets/Textures/Scenes/clouds.png");
	

		app->sceneManager->currentScene = this;

	}
	return true;
}

bool Scene1::Update(float dt)
{	
	if ((CheckWin() == 1) && (app->player->godMode == false))
	{
		app->fade->Fade(this, (Scene*)app->sceneManager->scene2, 1/dt);
	}

	else if ((CheckWin() == 2) && (app->player->godMode == false))
	{
		app->fade->Fade(this, (Scene*)app->sceneManager->deadScene, 1 / dt);
		app->player->Disable();
		app->sceneManager->lastScene = this;
	}

	app->sceneManager->checkpointKeepAnim.speed = 8.0f * dt;

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
	app->itemManager->Draw();

	if (checkpoint1 == true)
	{
		if (currentAnim != &app->sceneManager->checkpointKeepAnim)
		{
			app->sceneManager->checkpointKeepAnim.Reset();
			currentAnim = &app->sceneManager->checkpointKeepAnim;
		}
		app->render->DrawTexture(app->sceneManager->checkpointTexture, 1535, 155, &currentAnim->GetCurrentFrame());
		currentAnim->Update();
	}
	else if (checkpoint2 == true)
	{
		if (currentAnim != &app->sceneManager->checkpointKeepAnim)
		{
			app->sceneManager->checkpointKeepAnim.Reset();
			currentAnim = &app->sceneManager->checkpointKeepAnim;
		}
		app->render->DrawTexture(app->sceneManager->checkpointTexture, 2256, 268, &currentAnim->GetCurrentFrame());
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

	app->enemyManager->DeleteColliders();
	app->enemyManager->enemies.Clear();
	app->enemyManager->CleanUp();
	
	app->itemManager->DeleteColliders();
	app->itemManager->items.Clear();
	app->itemManager->CleanUp();

	return true;
}

bool Scene1::RestartPlayerPosition()
{
	if (checkpoint1 == true)
	{
		app->player->SetPosition(1535, 176);
	}
	else if (checkpoint2 == true)
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

			if (playerMidTile == 1167 && checkpoint2 == false)
			{
				checkpoint1 = true;
				currentAnim = &app->sceneManager->checkpointKeepAnim;
				if (checkSound1 == false)
				{
					app->audio->PlayFx(app->sceneManager->checkpointFx);
					checkSound1 = true;
					checkSound2 = false;
				}
			}
			else if (playerMidTile == 1168)
			{
 				checkpoint2 = true;
				checkpoint1 = false;
				if (checkSound2 == false)
				{
					app->audio->PlayFx(app->sceneManager->checkpointFx);
					checkSound1 = false;
					checkSound2 = true;
				}
			}
		}
	
		layer = layer->next;
	}
	
	return -1;
}
