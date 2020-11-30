#include "App.h"
#include "Log.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Scene2.h"
#include "Player.h"
#include "FadeToBlack.h"

Scene2::Scene1()
{
	name.Create("scene2");
}

Scene2::~Scene1()
{
}

bool Scene2::Start()
{
	sky = app->tex->Load("Assets/textures/sky.png");
	sea = app->tex->Load("Assets/textures/sea.png");
	clouds = app->tex->Load("Assets/textures/clouds.png");

	//playerStartPosition() 
	return true;
}

bool Scene2::Update(float dt)
{
	if (app->player->godMode == false)
	{
		switch (CheckWin())
		{
			//case 1:

		}
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

	if (app->map->active == true)
		app->map->Draw();

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

bool Scene2::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(sky);
	app->tex->UnLoad(clouds);
	app->tex->UnLoad(sea);

	return true;
}

bool Scene2::RestartPlayerPosition()
{
	app->player->SetPosition(playerStartPosition.x, playerStartPosition.y);

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
		}

		layer = layer->next;
	}

	return -1;
}
