#ifndef __MODULE_ENEMIES_H__
#define __MODULE_ENEMIES_H__

#include "Module.h"
#include "Enemy.h"

#define MAX_ENEMIES 100

enum class ENEMY_TYPE
{
	NO_TYPE,
	REDBIRD,
	BROWNSHIP,
	VERYBIGBALLOON,
	VERYBIGBALLOON2,
	BIGBALLOON,
	BIGBALLOON2,
	SMALLBALLOON,
	SMALLBALLOON2,
	VERYSMALLBALLOON,
	VERYSMALLBALLOON2,
	MECH,

};

struct EnemySpawnpoint
{
	ENEMY_TYPE type = ENEMY_TYPE::NO_TYPE;
	int x, y;
};

class Enemy;
struct SDL_Texture;
struct Enemy_Balloon;

class Enemies : public Module
{
public:
	// Constructor
	Enemies();

	// Virtual destructor
	virtual ~Enemies();

	// Called when the module is activated
	// Loads the necessary textures for the enemies
	bool Start() override;

	// Called at the middle of the application loop
	// Handles all enemies logic and spawning/despawning
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Iterates all the enemies and draws them
	bool PostUpdate() override;

	// Called on application exit
	// Destroys all active enemies left in the array
	bool CleanUp() override;

	// Called when an enemy collider hits another collider
	// The enemy is destroyed and an explosion particle is fired
	//void OnCollision(Collider* c1, Collider* c2) override;

	// Add an enemy into the queue to be spawned later
	bool AddEnemy(ENEMY_TYPE type, int x, int y);

	// Iterates the queue and checks for camera position
	void HandleEnemiesSpawn();

	// Destroys any enemies that have moved outside the camera limits
	void HandleEnemiesDespawn();

	// The enemies sprite sheet
	SDL_Texture* texture = nullptr;

	//Destroy all ballons till verysmall
	void destroyDynamiteBalloons();

private:
	// Spawns a new enemy using the data from the queue
	void SpawnEnemy(const EnemySpawnpoint& info);

private:
	// A queue with all spawn points information
	EnemySpawnpoint spawnQueue[MAX_ENEMIES];

	// All spawned enemies in the scene
	Enemy* enemies[MAX_ENEMIES] = { nullptr };

	Collider* balloonCollider = balloon.collider;

	// The audio fx for destroying an enemy
	int enemyDestroyedFx = 0;
};

#endif // __MODULE_ENEMIES_H__