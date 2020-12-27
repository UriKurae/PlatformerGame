﻿#include "App.h"
#include "Window.h"
#include "Fonts.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "Map.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Executioner.h"
#include "GreenGem.h"
#include "RedHeart.h"
#include "Wolf.h"
#include "SceneManager.h"
#include "EntityManager.h"

#include "Log.h"

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
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, iPoint(250, 5));
		player->Start();

		app->map->active = true;
		app->map->Load("level_1.tmx");

		if ((player->loadedGame) && (app->sceneManager->savedScene == this))
			player->SetPosition(player->savedPosition.x, player->savedPosition.y);

		else playerStartPosition = player->SetPosition(250, 20);

		if (player->loadedGame == false)
		{
			//executioners.Add((Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(400, 100)));
			//executioners.Add((Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(600, 100)));
			executioners.Add((Executioner*)app->entityManager->CreateEntity(EntityType::EXECUTIONER, iPoint(400, 100)));

			wolfs.Add((Wolf*)app->entityManager->CreateEntity(EntityType::WOLF, iPoint(400, 250)));
			wolfs.Add((Wolf*)app->entityManager->CreateEntity(EntityType::WOLF, iPoint(650, 260)));
			
			// Call the start method for all the enemies
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


			//==================================================================================

			//Items instantiation

			gems.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(1200, 140)));
			gems.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(1642, 96)));
			gems.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(2144, 512)));
			gems.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(2976, 544)));

			hearts.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(432, 176)));
			hearts.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(2080, 224)));
			hearts.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(2960, 304)));

		}
		else
		{
			ListItem<iPoint>* wolfItem = app->sceneManager->wolfSavedPositions.start;
			while (wolfItem != nullptr)
			{
				wolfs.Add((Wolf*)app->entityManager->CreateEntity(EntityType::WOLF, wolfItem->data));
				wolfItem = wolfItem->next;
			}
			
			ListItem<iPoint>* execItem = app->sceneManager->executionerSavedPositions.start;
			while (execItem != nullptr)
			{
				executioners.Add((Executioner*)app->entityManager->CreateEntity(EntityType::EXECUTIONER, execItem->data));
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

		// Gems initialization
		ListItem<GreenGem*>* gItem = gems.start;
		while (gItem != nullptr)
		{
			gItem->data->Start();
			gItem = gItem->next;
		}

		// Hearts initialization
		ListItem<RedHeart*>* hItem = hearts.start;
		while (hItem != nullptr)
		{
			hItem->data->Start();
			hItem = hItem->next;
		}

		// Assets loading and playing
		app->audio->PlayMusic("Assets/Audio/Music/scene_1.ogg");
		sky = app->tex->Load("Assets/Textures/Scenes/sky.png");
		sea = app->tex->Load("Assets/Textures/Scenes/sea.png");
		clouds = app->tex->Load("Assets/Textures/Scenes/clouds.png");
		GuiHeartTexture = app->tex->Load("Assets/Textures/Collectibles/collectibles.png");


		app->sceneManager->currentScene = this;
	}

	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!✕-:©✕ " };
	uiIndex = app->fonts->Load("Assets/Textures/fonts.png", lookupTable, 1);

	return true;
}

bool Scene1::Update(float dt)
{	
	app->sceneManager->checkpointKeepAnim.speed = 8.0f * dt;

	// Enemies machine states
	ListItem<Executioner*>* eItem = executioners.start;
	while (eItem != nullptr)
	{
		if (eItem->data->life > 0)
		{
			if (eItem->data->currentState == EnemyState::PATROL)
			{
				eItem->data->currentAnim = &eItem->data->idleAnim;

				if ((eItem->data->Patrol(dt, player->GetPosition())) && (player->GetReachable()) && (player->godMode == false))
					eItem->data->currentState = EnemyState::ALERT;
			}
			else if (eItem->data->currentState == EnemyState::ALERT)
			{
				if (eItem->data->FindTarget(player, dt))
					eItem->data->currentState = EnemyState::ATTACK;
			}
			else if (eItem->data->currentState == EnemyState::ATTACK)
			{
				if (eItem->data->ChaseTarget(dt))
				{
					eItem->data->path.Clear();
					eItem->data->currentState = EnemyState::PATROL;
				}
			}
		}

		eItem = eItem->next;
	}


	ListItem<Wolf*>* wItem = wolfs.start;
	while (wItem != nullptr)
	{
		if (wItem->data->life > 0)
		{
			if (wItem->data->currentState == EnemyState::PATROL)
			{
				if ((wItem->data->Patrol(dt, player->GetPosition())) && (player->GetReachable()) && (player->godMode == false))
					wItem->data->currentState = EnemyState::ALERT;
			}
			else if (wItem->data->currentState == EnemyState::ALERT)
			{
				if (wItem->data->FindTarget(player, dt))
					wItem->data->currentState = EnemyState::ATTACK;
			}
			else if (wItem->data->currentState == EnemyState::ATTACK)
			{
				if (wItem->data->ChaseTarget(dt))
				{
					wItem->data->path.Clear();
					wItem->data->currentState = EnemyState::PATROL;
				}
			}
		}

		wItem = wItem->next;
	}

	
	//for (uint i = 0; i < app->map->data.height; ++i)
	//{
	//	for (uint j = 0; i < app->map->data.width; ++j)
	//	{
	//		if ((app->map->data.layers[2]->Get(j, i) >= 1161) &&
	//			CheckCollisions(app->map->GetTileRect(j, i), player->GetSize()))
	//		{
	//			// TODO: Implement the player movement when collides

	//		}
	//	}
	//}

	// Check if the player picked a gem
	ListItem<GreenGem*>* gItem = gems.start;
	while (gItem != nullptr)
	{
		if (player->GetCollider()->Intersects(gItem->data->collider->rect))
		{
			player->PickItem(gItem->data->type);
			gItem->data->CleanUp();
		}
		gItem = gItem->next;
	}

	// Check if the player picked a heart
	ListItem<RedHeart*>* hItem = hearts.start;
	while (hItem != nullptr)
	{
		if (player->GetCollider()->Intersects(hItem->data->collider->rect))
		{
			player->PickItem(hItem->data->type);
			hItem->data->CleanUp();
		}
		hItem = hItem->next;
	}

	if ((CheckWin() == 1) && (player->godMode == false))
		TransitionToScene(app->sceneManager->scene2);
		//app->fade->Fade(this, (Scene*)app->sceneManager->scene2, 1/dt);

	else if ((CheckWin() == 2) && (player->godMode == false))
	{
		TransitionToScene((Scene*)app->sceneManager->deadScene);
		//app->fade->Fade(this, (Scene*)app->sceneManager->deadScene, 1 / dt);
		player->DisableEntity();
		app->sceneManager->lastScene = this;
	}

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		if (checkpoint1 == true || checkpoint2 == true)
			RestartPlayerPosition();
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
		app->map->Draw(player->GetPosition());
	
	player->Draw();
	app->entityManager->Draw();

	if (checkpoint1 == true)
	{
		if (currentAnimCheckpoint != &app->sceneManager->checkpointKeepAnim)
		{
			app->sceneManager->checkpointKeepAnim.Reset();
			currentAnimCheckpoint = &app->sceneManager->checkpointKeepAnim;
		}
		app->render->DrawTexture(app->sceneManager->checkpointTexture, 1535, 155, &currentAnimCheckpoint->GetCurrentFrame());
		currentAnimCheckpoint->Update();
	}
	else if (checkpoint2 == true)
	{
		if (currentAnimCheckpoint != &app->sceneManager->checkpointKeepAnim)
		{
			app->sceneManager->checkpointKeepAnim.Reset();
			currentAnimCheckpoint = &app->sceneManager->checkpointKeepAnim;
		}
		app->render->DrawTexture(app->sceneManager->checkpointTexture, 2256, 268, &currentAnimCheckpoint->GetCurrentFrame());
		currentAnimCheckpoint->Update();
	}

	DrawGui();

	app->fonts->DrawText(20, 20, uiIndex, "mamaguevo");

	return ret;
}

void Scene1::DrawGui()
{
	for (uint i = 1; i <= player->lifes; ++i)
	{
		SDL_Rect r = { 176,79,16,16 };
		app->render->DrawTexture(GuiHeartTexture, (-app->render->camera.x / app->win->GetScale()) + 20 * i, (-app->render->camera.y / app->win->GetScale()) + 5, &r);
	}
}

bool Scene1::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	app->map->CleanUp();

	player->DisableEntity();

	app->entityManager->ClearLists();
	//Should call entity manager cleanup?

	wolfs.Clear();
	executioners.Clear();
	gems.Clear();
	hearts.Clear();
	
	// Entity manager handles it
	/*app->itemManager->DeleteColliders();
	app->itemManager->items.Clear();
	app->itemManager->CleanUp();*/

	return true;
}

bool Scene1::RestartPlayerPosition()
{
	if (checkpoint1 == true && checkpoint2 == false)
		player->SetPosition(1535, 176);
	
	else if (checkpoint2 == true)
		player->SetPosition(2256, 272);

	else player->SetPosition(250, 70);

	return true;
}

int Scene1::CheckWin()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	
	iPoint playerPosTop = app->map->WorldToMap(player->GetPosition().x + 8, player->GetPosition().y + 15);
	
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
				currentAnimCheckpoint = &app->sceneManager->checkpointKeepAnim;
				
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

bool Scene1::CheckCollisions(SDL_Rect& a, SDL_Rect& b)
{
	return ((a.x < b.x + b.w) && (a.x + a.w > b.x) &&
			(a.y < b.y + b.h) && (a.h + a.y > b.y));
}


bool Scene1::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
		case GuiControlType::BUTTON:
		{
			if (control->id == 1) app->sceneManager->isPaused = false; // Resume
			else if (control->id == 2) app->sceneManager->statusMenu = MenuState::OPTIONS; // Settings
			else if (control->id == 3) // Back to title
			{
				TransitionToScene((Scene*)app->sceneManager->mainMenu);
				app->sceneManager->isPaused = false;
			}
			else if (control->id == 4) toExit = true; // Exit
			else if(control->id == 5) app->sceneManager->statusMenu = MenuState::INITIAL; // Button back from options
			break;
		}

		case GuiControlType::SLIDER:
		{
			if (control->id == 1) app->audio->SetMusicVolume(app->sceneManager->sliderMusicVolume->GetValue()); // Music volume
			else if (control->id == 2) app->audio->SetFXVolume(app->sceneManager->sliderFxVolume->GetValue()); // FX volume
			break;
		}

		case GuiControlType::CHECKBOX:
		{
			if (control->id == 1) app->win->fullscreenWindow = !app->win->fullscreenWindow; // FullScreen
			else if (control->id == 2) app->render->vsync = !app->render->vsync; // Vsync
			break;
		}
		default: break;
	}

	return true;
}