#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Fonts.h"
#include "Textures.h"
#include "Audio.h"
#include "Map.h"
#include "EntityManager.h"
#include "SceneManager.h"
//#include "ItemManager.h"
#include "Pathfinding.h"
//#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	//collisions = new Collisions();
	map = new Map();
	entityManager = new EntityManager();
	sceneManager = new SceneManager();
	//itemManager = new ItemManager();
	pathFinding = new PathFinding();
	fonts = new Fonts();
	//fade = new FadeToBlack();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win, true);
	AddModule(input,true);
	AddModule(tex, true);
	AddModule(fonts, true);
	AddModule(audio, true);
	AddModule(map, false);
	AddModule(sceneManager, true);
	AddModule(entityManager, true);
	//AddModule(itemManager, true);
	AddModule(pathFinding, true);
	//AddModule(collisions, true);
	//AddModule(fade, true);

	// Render last to swap buffer
	AddModule(render, true);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module, bool active)
{
	module->Init(active);
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;
	
	bool ret = false;
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
		cappedMs = configApp.attribute("framerate_cap").as_float();

	}

	if(ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	PERF_START(ptimer);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();


	return ret;
}

// Load config from XML file
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	++frameCount;
	++lastSecFrameCount;

	if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		changeMs = !changeMs;

		if (changeMs)
		{
			cappedMs = 1000 / 30;
		}

		else if (!changeMs)
			cappedMs = 1000 / 60;
	}
	
	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{

	if (saveGameRequested)
		SaveGame();
	if (loadGameRequested)
		LoadGame();

	// Frame rate and dt control

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float averageFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	SString vsyncChar;
	if(render->vsync) vsyncChar.Create("on");
	else vsyncChar.Create("off");

	static char title[256];
	sprintf_s(title, 256, "FPS: %i / Av.FPS: %.2f / Last Frame Ms: %02u / Vsync: %s", framesOnLastUpdate, averageFps, lastFrameMs, vsyncChar.GetString());

	app->win->SetTitle(title);

	if ((cappedMs > 0) && (lastFrameMs < cappedMs))
	{
		PERF_START(ptimer);
		SDL_Delay(cappedMs - lastFrameMs);
		//LOG("We waited for %i ms and got back in %f", cappedMs, ptimer.ReadMs());
	}

}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) 
		{
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::RequestLoadGame()
{
	loadGameRequested = true;
}

void App::RequestSaveGame()
{
	saveGameRequested = true;
}


bool App::LoadGame()
{
	loadGameRequested = false;
	bool ret = true;

	pugi::xml_parse_result resul = saveLoadFile.load_file("save_game.xml");

	if (resul == NULL)
	{
		LOG("Could not load save and load xml file. pugi error: %s", resul.description());
		return false;
	}
	else
	{
		saveState = saveLoadFile.child("save_status");

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret)
		{
			ret = item->data->Load(saveState.child(item->data->name.GetString()));
			item = item->next;
		}
		
		LOG("File loaded successfully!");
	}

	return true;
}

bool App::SaveGame()
{
	LOG("Saving Results!!");

	saveGameRequested = false;
	bool ret = true;

	ListItem<Module*>* item = modules.start;
	pugi::xml_document file;

	auto root = file.append_child("save_status");

	while (item != NULL)
	{
		root.append_child(item->data->name.GetString());
		ret = item->data->Save(root.child(item->data->name.GetString()));
		item = item->next;
	}

	bool saveSucceed = file.save_file("save_game.xml", PUGIXML_TEXT("  "));
	if (saveSucceed == false)
	{
		LOG("Couldn't save the file. pugi error: %s", pugi::status_internal_error);
	}

	return ret;
}