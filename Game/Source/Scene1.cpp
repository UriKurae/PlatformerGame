#include "App.h"
#include "Log.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Scene1.h"
#include "SceneManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Executioner.h"
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

		player = new Player(iPoint(250, 0));
		player->Start();

		executioner = (Executioner*)app->enemyManager->AddEnemy(EnemyType::EXECUTIONER, iPoint(250, 200));
		executioner->Start();

		sky = app->tex->Load("Assets/textures/sky.png");
		sea = app->tex->Load("Assets/textures/sea.png");
		clouds = app->tex->Load("Assets/textures/clouds.png");

		//playerStartPosition()  
	}
	return true;
}

bool Scene1::Update(float dt)
{	
	if (CheckWin() == 1)
	{
		app->fade->Fade(this, (Scene*)app->sceneManager->scene2, 1/dt);
	}

	player->Update(dt);

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
	player->Draw();

	return ret;

	//if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	//{
	//	app->pathFinding->PropagateBFS();
	//	//app->map->PropagateDijkstra();
	//	//app->map->PropagateAStar(1);
	//	//app->pathFinding->PropagateAStar(1);
	//}
	//if (app->input->GetKey(SDL_SCANCODE_R) == KEY_UP)
	//{
	//	iPoint playerPos(app->player->GetPosition().x / 16, app->player->GetPosition().y / 16);
	//	app->pathFinding->ResetPath(playerPos);
	//}
	//	
	//app->pathFinding->DrawPath();
}

bool Scene1::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	player->CleanUp();
	app->map->CleanUp();


	RELEASE(player);

	executioner->CleanUp();
	RELEASE(executioner);

	return true;
}

bool Scene1::RestartPlayerPosition()
{
	player->SetPosition(playerStartPosition.x, playerStartPosition.y);

	return true;
}

int Scene1::CheckWin()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	
	iPoint playerPosTop = app->map->WorldToMap(player->GetPosition().x+8, player->GetPosition().y+15);
	
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
