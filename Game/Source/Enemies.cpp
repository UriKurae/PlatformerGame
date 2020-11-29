#include "Enemies.h"

#include "App.h"

#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Player.h"
#include "Collisions.h"

#include "Enemy.h"

#include "Log.h"

#include "SDL/include/SDL.h"

#define SPAWN_MARGIN 50


Enemies::Enemies() : Module()
{
	name = "ENEMIES";

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

bool Enemies::Start()
{
	texture = app->tex->Load("Assets/Balloons/Balloons.png");


	return true;
}

bool Enemies::Update(float dt)
{
	HandleEnemiesSpawn();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
			enemies[i]->Update(dt);
	}

	HandleEnemiesDespawn();

	return true;
}

bool Enemies::PostUpdate()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Draw();
	}

	return true;
}

// Called before quitting
bool Enemies::CleanUp()
{
	LOG("Freeing all enemies");

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	SDL_DestroyTexture(texture);

	return true;
}

bool Enemies::AddEnemy(ENEMY_TYPE type, int x, int y)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		//App->enemies[i].touchWall = false;

		if(spawnQueue[i].type == ENEMY_TYPE::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void Enemies::HandleEnemiesSpawn()
{
	uint x, y;
	app->win->GetWindowSize(x, y);
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type != ENEMY_TYPE::NO_TYPE)
		{
			// Spawn a new enemy if the screen has reached a spawn position
			if (spawnQueue[i].x * x < app->render->camera.x + (app->render->camera.w * x) + SPAWN_MARGIN)
			{
				LOG("Spawning enemy at %d", spawnQueue[i].x * SCREEN_SIZE);

				SpawnEnemy(spawnQueue[i]);
				spawnQueue[i].type = ENEMY_TYPE::NO_TYPE; // Removing the newly spawned enemy from the queue
			}
		}
	}
}

void Enemies::HandleEnemiesDespawn()
{
	uint x, y;
	app->win->GetWindowSize(x, y);
	// Iterate existing enemies
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			// Delete the enemy when it has reached the end of the screen
			if (enemies[i]->position.x * x < (app->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);

				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}
}

void Enemies::SpawnEnemy(const EnemySpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			switch (info.type)
			{
				
				/*case ENEMY_TYPE::VERYBIGBALLOON:
					enemies[i] = new Enemy_Balloon(info.x, info.y, ENEMY_TYPE::VERYBIGBALLOON);
					break;
				*/
			}
			enemies[i]->texture = texture;
			break;
		}
	}
}

/*void Enemies::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->OnCollision(c2); //Notify the enemy of a collision

			if (c2->type != Collider::TYPE::WALL && c2->type != Collider::TYPE::BREAKABLE_BLOCK) {
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			}
		}
	}
}*/