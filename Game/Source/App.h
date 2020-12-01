#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class IntroScene;
class Scene;
class Scene2;
class WinScene;
class Map;
class Player;
class SceneManager;
class EnemyManager;
class Collisions;
class FadeToBlack;
class DeadScene;
class PathFinding;


class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module, bool active);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void RequestLoadGame();
	void RequestSaveGame();

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load game method
	bool LoadGame();

	// Save game method
	bool SaveGame();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	IntroScene* intro;
	Scene* scene;
	Scene2* scene2;
	WinScene* winScene;
	DeadScene* deadScene;
	Map* map;
	Player* player;
	SceneManager* sceneManager;
	EnemyManager* enemyManager;
	Collisions* collisions;
	FadeToBlack* fade;
	PathFinding* pathFinding;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	bool saveGameRequested;
	bool loadGameRequested;

	pugi::xml_document saveLoadFile;
	pugi::xml_node saveState;

	PerfTimer ptimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;

	int cappedMs = -1;
	bool changeMs = false;

};

extern App* app;

#endif	// __APP_H__