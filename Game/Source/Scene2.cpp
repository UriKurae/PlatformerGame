#include "App.h"
#include "Window.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Scene2.h"
#include "SceneManager.h"
#include "Player.h"
#include "Executioner.h"
#include "GreenGem.h"
#include "RedHeart.h"
#include "Wolf.h"
#include "EntityManager.h"
#include "Fonts.h"

#include "Log.h"

Scene2::Scene2()
{
	name.Create("scene2");

	// Pushbacks for timers
	timerAnimation.PushBack({ 0,125,25,24 });
	timerAnimation.PushBack({ 25,125,25,24 });
	timerAnimation.PushBack({ 50,125,25,24 });
	timerAnimation.PushBack({ 75,125,25,24 });
	timerAnimation.PushBack({ 100,125,25,24 });
	timerAnimation.PushBack({ 125,125,25,24 });
	timerAnimation.PushBack({ 150,125,25,24 });
	timerAnimation.PushBack({ 175,125,25,24 });
	timerAnimation.PushBack({ 200,125,25,24 });

	timerAnimation.loop = true;

	// Pushbacks for Hearts
	heartAnimation.PushBack({ 121, 79, 16, 16 });
	heartAnimation.PushBack({ 139, 79, 16, 16 });
	heartAnimation.PushBack({ 157, 79, 16, 16 });
	heartAnimation.PushBack({ 176, 79, 16, 16 });
	heartAnimation.PushBack({ 121, 99, 16, 16 });
	heartAnimation.PushBack({ 139, 99, 16, 16 });
	//heartAnimation.PushBack({ 157, 99, 16, 16 });
	//heartAnimation.PushBack({ 176, 99, 16, 16 });
	heartAnimation.loop = true;

	// Pushbacks for gems
	gemAnimation.PushBack({ 2,2,10,15 });
	gemAnimation.PushBack({ 18,2,10,15 });
	gemAnimation.PushBack({ 34,2,10,15 });
	gemAnimation.PushBack({ 50,2,10,15 });
	gemAnimation.loop = true;
}

Scene2::~Scene2()
{
}

bool Scene2::Start()
{
	if (this->active)
	{
		app->entityManager->Start();

		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, iPoint(250, 40));
		player->Start();
	
		app->map->active = true;
		app->map->Load("level_2.tmx");

		sky = app->tex->Load("Assets/Textures/Scenes/sky.png");
		sea = app->tex->Load("Assets/Textures/Scenes/sea.png");
		clouds = app->tex->Load("Assets/Textures/Scenes/clouds.png");

		app->audio->PlayMusic("Assets/Audio/Music/scene_2.ogg");

		if ((player->loadedGame) && (app->sceneManager->savedScene == this))
			player->SetPosition(player->savedPosition.x, player->savedPosition.y);

		else
			playerStartPosition = player->SetPosition(250, 5);

		if (player->loadedGame == false)
		{
			executioners.Add((Executioner*)app->entityManager->CreateEntity(EntityType::EXECUTIONER, iPoint(528, 290)));

			wolfs.Add((Wolf*)app->entityManager->CreateEntity(EntityType::WOLF, iPoint(288, 323)));
			wolfs.Add((Wolf*)app->entityManager->CreateEntity(EntityType::WOLF, iPoint(800, 323)));

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

		// Items instantiation and initialization
		gems.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(544, 304)));
		gems.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(2144, 336)));
		gems.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(1136, 112)));
		gems.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(3504, 224)));

		hearts.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(1296, 480)));
		hearts.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(2496, 224)));
		hearts.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(3120, 272)));

		ListItem<GreenGem*>* gItem = gems.start;
		while (gItem != nullptr)
		{
			gItem->data->Start();
			gItem = gItem->next;
		}

		ListItem<RedHeart*>* hItem = hearts.start;
		while (hItem != nullptr)
		{
			hItem->data->Start();
			hItem = hItem->next;
		}
	}

	app->sceneManager->currentScene = this;
	guiDebugDraw = false;

	guiTexture = app->tex->Load("Assets/Textures/Collectibles/collectibles.png");

	char lookupTable[] = { "!,-.0123456789?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz " };
	uiIndex = app->fonts->Load("Assets/Textures/fonts.png", lookupTable, 1);

	return true;
}

bool Scene2::Update(float dt)
{
	app->sceneManager->checkpointKeepAnim.speed = 8.0f * dt;

	timerAnimation.speed = 9.5f * dt;
	timer += 1.0f * dt;

	heartAnimation.speed = 5.0f * dt;

	gemAnimation.speed = 4.0f * dt;

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
			gems.Del(gItem);
			break;
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
			hearts.Del(hItem);
			break;
		}
		hItem = hItem->next;
	}

	if ((CheckWin() == 1) && (player->godMode == false))
		TransitionToScene((Scene*)app->sceneManager->winScene);

	else if ((CheckWin() == 2) && (player->godMode == false))
	{
		deadOnScene = true;
		TransitionToScene((Scene*)app->sceneManager->deadScene);
		app->sceneManager->lastScene = this;
		player->DisableEntity();
	}

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		{
			if (checkpointItem != nullptr)
			{
				player->SetPosition(checkpointItem->data.x, checkpointItem->data.y);

				if (checkpointItem->next != nullptr)
					checkpointItem = checkpointItem->next;
				else
					checkpointItem->next = checkpoints.start;
			}
		}
	}

	if (currentAnimCheckpoint != nullptr)
		currentAnimCheckpoint->Update();

	if (currentAnimTimer != nullptr)
		currentAnimTimer->Update();

	if (currentAnimGem != nullptr)
		currentAnimGem->Update();

	if (currentAnimHeart != nullptr)
		currentAnimHeart->Update();

	sprintf_s(timerText, 10, "%.0f", timer);

	return true;
}

bool Scene2::Draw()
{
	bool ret = true;

	app->render->DrawTexture(sky, -200, -60, NULL, 0.65f);
	app->render->DrawTexture(clouds, -200, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, -200, 395, NULL, 0.85f);

	if (app->map->active)
		app->map->Draw(player->GetPosition());

	player->Draw();

	app->entityManager->Draw();
	
	ListItem<iPoint>* item = checkpoints.start;
	while (item != nullptr)
	{
		app->render->DrawTexture(app->sceneManager->checkpointTexture, item->data.x, item->data.y - 19, &currentAnimCheckpoint->GetCurrentFrame());
		item = item->next;
	}

	DrawGui();

	return ret;
}

void Scene2::DrawGui()
{
	for (uint i = 1; i <= player->lifes; ++i)
	{
		if (currentAnimHeart != nullptr)
			app->render->DrawTexture(guiTexture, (-app->render->camera.x / app->win->GetScale()) + 20 * i, (-app->render->camera.y / app->win->GetScale()) + 5, &currentAnimHeart->GetCurrentFrame());
	}

	for (uint i = 1; i <= player->gemsAchieved; ++i)
	{
		if (currentAnimGem != nullptr)
			app->render->DrawTexture(guiTexture, (-app->render->camera.x / app->win->GetScale()) + 20 * i, (-app->render->camera.y / app->win->GetScale()) + 25, &currentAnimGem->GetCurrentFrame());
	}

	if (currentAnimTimer != &timerAnimation)
	{
		timerAnimation.Reset();
		currentAnimTimer = &timerAnimation;
	}

	if (currentAnimGem != &gemAnimation)
	{
		gemAnimation.Reset();
		currentAnimGem = &gemAnimation;
	}

	if (currentAnimHeart != &heartAnimation)
	{
		heartAnimation.Reset();
		currentAnimHeart = &heartAnimation;
	}

	app->render->DrawRectangle({ (-app->render->camera.x / (int)app->win->GetScale()) + 608, (-app->render->camera.y / (int)app->win->GetScale()) + 8, 25, 17 }, { 255,177,020,195 });

	app->render->DrawTexture(guiTexture, (-app->render->camera.x / app->win->GetScale()) + 580, (-app->render->camera.y / app->win->GetScale()) + 5, &currentAnimTimer->GetCurrentFrame());
	app->fonts->DrawText(613, 10, uiIndex, timerText);
}

bool Scene2::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);
	app->tex->UnLoad(guiTexture);

	app->map->CleanUp();
	player->DisableEntity();

	app->entityManager->DeleteColliders();
	app->entityManager->CleanUp();
	app->entityManager->ClearLists();

	wolfs.Clear();
	executioners.Clear();
	gems.Clear();
	hearts.Clear();

	return true;
}

bool Scene2::RestartPlayerPosition()
{
	if (checkpoint1 == true)
		player->SetPosition(1552, 464);

	else if (checkpoint2 == true)
		player->SetPosition(3024, 208);

	else player->SetPosition(250, 70);

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
				player->blockCamera = true;
				return 1;
			}

			if (playerMidTile == 1170)
			{
				player->blockCamera = true;
				return 2;
			}

			if (playerMidTile == 1167)
			{
				if (checkpoints.Find(iPoint(1552, 464)) == -1)
				{
					checkpoints.Add(iPoint(1552, 464));
					checkpointItem = checkpoints.start;
					app->audio->PlayFx(app->sceneManager->checkpointFx);
				}
				currentAnimCheckpoint = &app->sceneManager->checkpointKeepAnim;
			}
			else if (playerMidTile == 1168)
			{
				if (checkpoints.Find(iPoint(3024, 224)) == -1)
				{
					checkpoints.Add(iPoint(3024, 224));
					checkpointItem = checkpoints.start;
					app->audio->PlayFx(app->sceneManager->checkpointFx);
				}
				currentAnimCheckpoint = &app->sceneManager->checkpointKeepAnim;
			}
		}

		layer = layer->next;
	}

	return -1;
}

bool Scene2::OnGuiMouseClickEvent(GuiControl* control)
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
		else if (control->id == 5) app->sceneManager->statusMenu = MenuState::INITIAL; // Button back from options
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