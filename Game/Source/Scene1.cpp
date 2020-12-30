#include "App.h"
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
#include "Pathfinding.h"

#include "Log.h"

Scene1::Scene1()
{
	name.Create("scene1");

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
	heartAnimation.loop = true;

	// Pushbacks for gems
	gemAnimation.PushBack({2,2,10,15});
	gemAnimation.PushBack({18,2,10,15});
	gemAnimation.PushBack({34,2,10,15});
	gemAnimation.PushBack({50,2,10,15});
	gemAnimation.loop = true;

}

Scene1::~Scene1()
{
}

bool Scene1::Start()
{
	if (this->active == true)
	{
		//app->entityManager->CleanUp();
		app->entityManager->Enable();
		app->pathFinding->Enable();

		timer = 0;

		app->map->active = true;
		app->map->Load("level_1.tmx");

		
		if (app->sceneManager->newGame == true)
		{
			player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, iPoint(250, 20));
			
			// Player initial position
			if (checkpoints.Count() > 0)
				player->position = checkpoints.end->data;
			else
				player->position = iPoint(250, 20);

			enemies.Add((Executioner*)app->entityManager->CreateEntity(EntityType::EXECUTIONER, iPoint(400, 100)));

			enemies.Add((Wolf*)app->entityManager->CreateEntity(EntityType::WOLF, iPoint(400, 250)));
			enemies.Add((Wolf*)app->entityManager->CreateEntity(EntityType::WOLF, iPoint(650, 260)));
		}
		else
		{
			pugi::xml_document tmp;


			pugi::xml_parse_result resul = tmp.load_file("save_game.xml");
			if (resul != NULL)
			{
				pugi::xml_node node = tmp.child("save_status").child("entitymanager");

				int numWolves = 0;
				int numExecutioners = 0;

				numExecutioners = node.child("num_enemies").child("executioners").attribute("value").as_int();
				numWolves = node.child("num_enemies").child("wolves").attribute("value").as_int();

				player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, iPoint(0, 0));

				while (numExecutioners > 0)
				{
					enemies.Add((Enemy*)app->entityManager->CreateEntity(EntityType::EXECUTIONER, iPoint(0, 0)));
					numExecutioners -= 1;
				}

				while (numWolves > 0)
				{
					enemies.Add((Enemy*)app->entityManager->CreateEntity(EntityType::WOLF, iPoint(0, 0)));
					numWolves -= 1;
				}

				ListItem<Entity*>* item = app->entityManager->entities.start;
				pugi::xml_node executioner = node.child("enemies").child("executioner");
				pugi::xml_node wolf = node.child("enemies").child("wolf");

				while (item != nullptr)
				{
					if (item->data->name == "player")
					{
						item->data->Load(node.child("player"));
					}
					else if (item->data->name == "executioner")
					{
						item->data->Load(executioner);
						executioner = executioner.next_sibling("executioner");
					}
					else if (item->data->name == "wolf")
					{
						item->data->Load(wolf);
						wolf = wolf.next_sibling("wolf");
					}

					item = item->next;
				}
			}		

			/*ListItem<Entity*>* item = app->entityManager->entities.start;

			while (item != nullptr)
			{
				if (item->data->name == "player")
				{
					player = (Player*)item->data;
				}
				else if (item->data->name == "executioner" || item->data->name == "wolf")
				{
					enemies.Add((Enemy*)item->data);
				}
				
				item = item->next;
			}*/
			app->sceneManager->newGame = true;
		}
	}

	//Items instantiation
	items.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(1200, 140)));
	items.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(1642, 96)));
	items.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(2144, 512)));
	items.Add((GreenGem*)app->entityManager->CreateEntity(EntityType::GEM, iPoint(2976, 544)));

	items.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(432, 176)));
	items.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(2080, 224)));
	items.Add((RedHeart*)app->entityManager->CreateEntity(EntityType::HEART, iPoint(2960, 304)));


	// Assets loading and playing
	app->audio->PlayMusic("Assets/Audio/Music/scene_1.ogg");
	sky = app->tex->Load("Assets/Textures/Scenes/sky.png");
	sea = app->tex->Load("Assets/Textures/Scenes/sea.png");
	clouds = app->tex->Load("Assets/Textures/Scenes/clouds.png");
	//guiTexture = app->tex->Load("Assets/Textures/Collectibles/collectibles.png");

	char lookupTable[] = { "!,-.0123456789?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz " };
	uiIndex = app->fonts->Load("Assets/Textures/fonts.png", lookupTable, 1);

	app->sceneManager->currentScene = this;
	guiDebugDraw = false;

	return true;
}

bool Scene1::Update(float dt)
{
	timerAnimation.speed = 9.5f * dt;
	timer += 1.0f * dt;

	heartAnimation.speed = 5.0f * dt;

	gemAnimation.speed = 4.0f * dt;


	// Enemies machine states

	int minX = (player->position.x - 300);
	int maxX = (player->position.x + 448);
	
	ListItem<Enemy*>* eItem = enemies.start;
	while (eItem != nullptr)
	{
		if ((eItem->data->position.x > minX) && (eItem->data->position.x < maxX))
		{
			if (eItem->data->life > 0)
			{
				if (eItem->data->currentState == EnemyState::PATROL)
				{
					//eItem->data->currentAnim = &eItem->data->idleAnim;

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
		}

		eItem = eItem->next;
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

	
	ListItem<Item*>* it = items.start;
	while (it != nullptr)
	{
		if (player->GetCollider()->Intersects(it->data->collider->rect))
		{
			player->PickItem(it->data->type);
			it->data->CleanUp();
			items.Del(it);
			break;
		}
		it = it->next;
	}

	if ((CheckWin() == 1) && (player->godMode == false))
		TransitionToScene(app->sceneManager->scene2);

	else if ((CheckWin() == 2) && (player->godMode == false))
	{
		TransitionToScene((Scene*)app->sceneManager->deadScene);
		app->sceneManager->lastScene = this;
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		if (checkpointItem != nullptr)
		{
			player->SetPosition(checkpointItem->data.x, checkpointItem->data.y);

			if (checkpointItem->next != nullptr)
				checkpointItem = checkpointItem->next;
			else
				checkpointItem = checkpoints.start;
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

bool Scene1::Draw()
{
	bool ret = true;
		
	app->render->DrawTexture(sky, -200, -80, NULL, 0.65f);
	app->render->DrawTexture(clouds, -200, 180, NULL, 0.75f);
	app->render->DrawTexture(sea, -200, 395, NULL, 0.85f);
	
	if(app->map->active == true)
		app->map->Draw(player->GetPosition());
	
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

void Scene1::DrawGui()
{
	for (int i = 1; i <= player->lifes; ++i)
	{
		if (currentAnimHeart != nullptr)
			app->render->DrawTexture(app->entityManager->collectiblesTexture, (-app->render->camera.x / app->win->GetScale()) + 20 * i, (-app->render->camera.y / (int)app->win->GetScale()) + 5, &currentAnimHeart->GetCurrentFrame());
	}

	for (int i = 1; i <= player->gemsAchieved; ++i)
	{
		if(currentAnimGem != nullptr)
			app->render->DrawTexture(app->entityManager->collectiblesTexture, (-app->render->camera.x / app->win->GetScale()) + 20 * i, (-app->render->camera.y / app->win->GetScale()) + 25, &currentAnimGem->GetCurrentFrame());
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
	
	app->render->DrawRectangle({ (-app->render->camera.x / (int)app->win->GetScale()) + 608, (-app->render->camera.y / (int)app->win->GetScale()) + 8, 25, 17 }, {255,177,020,195});
	
	app->render->DrawTexture(app->entityManager->collectiblesTexture, (-app->render->camera.x / app->win->GetScale()) + 580, (-app->render->camera.y / app->win->GetScale()) + 5, &currentAnimTimer->GetCurrentFrame());
	app->fonts->DrawText(613, 10, uiIndex, timerText);
}

bool Scene1::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	app->map->CleanUp();

	player->CleanUp();

	app->entityManager->DeleteEntities();
	app->entityManager->Disable();

	enemies.Clear();
	items.Clear();

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
	
			if ((playerMidTile == 1166) && (player->godMode == false))
			{
				player->blockCamera = true;
				return 1;
			}

			if ((playerMidTile == 1170) && (player->godMode == false))
			{
				player->blockCamera = true;
				return 2;
			}

			if (playerMidTile == 1167)
			{
				if (checkpoints.Find(iPoint(1520, 176)) == -1)
				{
					checkpoints.Add(iPoint(1520, 176));
					checkpointItem = checkpoints.start;
					app->audio->PlayFx(app->sceneManager->checkpointFx);
				}
				currentAnimCheckpoint = &app->sceneManager->checkpointKeepAnim;
			}

			else if (playerMidTile == 1168)
			{
				if (checkpoints.Find(iPoint(2256, 288)) == -1)
				{
					checkpoints.Add(iPoint(2256, 288));
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