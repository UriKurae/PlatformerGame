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

		sky = app->tex->Load("Assets/Textures/Scenes/sky.png");
		sea = app->tex->Load("Assets/Textures/Scenes/sea.png");
		clouds = app->tex->Load("Assets/Textures/Scenes/clouds.png");

		app->audio->PlayMusic("Assets/Audio/Music/scene_2.ogg");

		app->player->Enable();

		if ((app->player->loadedGame) && (app->sceneManager->savedScene == this))
			app->player->SetPosition(app->player->savedPosition.x, app->player->savedPosition.y);

		else
			playerStartPosition = app->player->SetPosition(250, 5);

		if (app->player->loadedGame == false)
		{
			executioners.Add((Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(528, 290)));

			wolfs.Add((Wolf*)app->enemyManager->AddEnemy(EnemyType::WOLF, iPoint(288, 323)));
			wolfs.Add((Wolf*)app->enemyManager->AddEnemy(EnemyType::WOLF, iPoint(800, 323)));

			ListItem<Wolf*>* itWolfs = wolfs.start;
			while (itWolfs != nullptr)
			{
				itWolfs->data->Start();
				itWolfs = itWolfs->next;
			}

			ListItem<Executioner*>* itExec = executioners.start;
			while (itExec != nullptr)
			{
				itExec->data->Start();
				itExec = itExec->next;
			}
		}
		else
		{
			ListItem<iPoint>* wolfItem = app->sceneManager->wolfSavedPositions.start;
			while (wolfItem != nullptr)
			{
				wolfs.Add((Wolf*)app->enemyManager->AddEnemy(EnemyType::WOLF, wolfItem->data));
				wolfItem = wolfItem->next;
			}

			ListItem<iPoint>* execItem = app->sceneManager->executionerSavedPositions.start;
			while (execItem != nullptr)
			{
				executioners.Add((Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, execItem->data));
				execItem = execItem->next;
			}

			// Call starts
			ListItem<Wolf*>* itWolfs = wolfs.start;
			while (itWolfs != nullptr)
			{
				itWolfs->data->Start();
				itWolfs = itWolfs->next;
			}

			ListItem<Executioner*>* itExec = executioners.start;
			while (itExec != nullptr)
			{
				itExec->data->Start();
				itExec = itExec->next;
			}
		}

		// Items instantiation
		gem1 = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(544, 304));
		gem1->Start();

		gem2 = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(2144, 336));
		gem2->Start();

		gem3 = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(1136, 112));
		gem3->Start();

		gem4 = (GreenGem*)app->itemManager->AddItem(ItemType::GEM, iPoint(3504, 224));
		gem4->Start();

		heart1 = (RedHeart*)app->itemManager->AddItem(ItemType::HEART, iPoint(1296, 480));
		heart1->Start();

		heart2 = (RedHeart*)app->itemManager->AddItem(ItemType::HEART, iPoint(2496, 224));
		heart2->Start();

		heart3 = (RedHeart*)app->itemManager->AddItem(ItemType::HEART, iPoint(3120, 272));
		heart3->Start();
	}

	app->sceneManager->currentScene = this;

	return true;
}

bool Scene2::Update(float dt)
{
	app->sceneManager->checkpointKeepAnim.speed = 8.0f * dt;

	if ((CheckWin() == 1) && (app->player->godMode == false))
		app->fade->Fade(this, (Scene*)app->sceneManager->winScene, 1 / dt);

	else if ((CheckWin() == 2) && (app->player->godMode == false))
	{
		deadOnScene = true;
		app->fade->Fade(this, (Scene*)app->sceneManager->deadScene, 1 / dt);
		app->sceneManager->lastScene = this;
		app->player->Disable();
	}

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		if (checkpoint1 == true || checkpoint2 == true)
			RestartPlayerPosition();
	}

	return true;
}

bool Scene2::Draw()
{
	bool ret = true;

	app->render->DrawTexture(sky, -200, -10, NULL, 0.65f);
	app->render->DrawTexture(clouds, -200, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, -200, 395, NULL, 0.85f);

	if (app->map->active)
		app->map->Draw();

	if(app->player->active)
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

		app->render->DrawTexture(app->sceneManager->checkpointTexture, 1552, 445, &currentAnim->GetCurrentFrame());
		currentAnim->Update();
	}
	else if (checkpoint2 == true)
	{
		if (currentAnim != &app->sceneManager->checkpointKeepAnim)
		{
			app->sceneManager->checkpointKeepAnim.Reset();
			currentAnim = &app->sceneManager->checkpointKeepAnim;
		}

		app->render->DrawTexture(app->sceneManager->checkpointTexture, 3024, 208, &currentAnim->GetCurrentFrame());
		currentAnim->Update();
	}

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

	app->enemyManager->DeleteColliders();
	app->enemyManager->enemies.Clear();
	wolfs.Clear();
	executioners.Clear();

	app->itemManager->DeleteColliders();
	app->itemManager->items.Clear();
	app->itemManager->CleanUp();

	return true;
}

bool Scene2::RestartPlayerPosition()
{
	if (checkpoint1 == true)
		app->player->SetPosition(1552, 464);

	else if (checkpoint2 == true)
		app->player->SetPosition(3024, 208);

	else app->player->SetPosition(250, 70);

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
				return 1;

			if (playerMidTile == 1170)
				return 2;

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
