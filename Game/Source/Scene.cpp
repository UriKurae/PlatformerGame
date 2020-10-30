#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	// L03: DONE: Load map
	app->map->Load("Level1.tmx");
	//bgImage = app->tex->Load("Assets/textures/bg.png");
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	sky = app->tex->Load("Assets/textures/sky2.png");
	sea = app->tex->Load("Assets/textures/sea2.png");
	clouds = app->tex->Load("Assets/textures/clouds2.png");
	playerStartPosition = app->player->SetPosition(230, 128);
	
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L02: TODO 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->RequestLoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->RequestSaveGame();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		app->audio->VolumeControl();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		RestartLevel();

	app->render->DrawTexture(sky, 0, 0, NULL);
	app->render->DrawTexture(clouds, 0, 100, NULL);
	app->render->DrawTexture(sea, 0, 265, NULL);
	

	app->map->Draw();


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::RestartLevel()
{
	app->player->SetPosition(playerStartPosition.x, playerStartPosition.y);

	return true;
}