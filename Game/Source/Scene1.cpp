#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Scene1.h"
#include "Player.h"
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
	sky = app->tex->Load("Assets/textures/sky.png");
	sea = app->tex->Load("Assets/textures/sea.png");
	clouds = app->tex->Load("Assets/textures/clouds.png");

	//playerStartPosition() 
	return true;
}

bool Scene1::Update(float dt)
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

bool Scene1::Draw()
{
	return true;
}

bool Scene1::CleanUp()
{
	return true;
}

bool Scene1::RestartPlayerPosition()
{
	return true;
}

int Scene1::CheckWin()
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	
	iPoint playerPosTop = app->map->WorldToMap(app->player->GetPosition().x+8, app->player->GetPosition().y+15);
	
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
