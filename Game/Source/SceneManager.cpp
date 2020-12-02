#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Map.h"
#include "SceneManager.h"
#include "FadeToBlack.h"
#include "IntroScene.h"
#include "Scene1.h"
#include "Scene2.h"
#include "DeadScene.h"


#include "SDL/include/SDL_scancode.h"

SceneManager::SceneManager()
{
	name.Create("scenemanager");

	introScene = new IntroScene();
	scene1 = new Scene1();
	scene2 = new Scene2();
	deadScene = new DeadScene();

	AddScene(introScene, false);
	AddScene(scene1, true);
	AddScene(scene2, false);
	AddScene(deadScene, false);
}

SceneManager::~SceneManager()
{
	scenes.Clear();
}

bool SceneManager::Start()
{
	ListItem<Scene*>* item = scenes.start;
	while (item != nullptr)
	{
		if(item->data->active == true)
			item->data->Start();
		
		item = item->next;
	}
	
	return true;
}

bool SceneManager::Update(float dt)
{
	bool ret = true;

	ret = HandleInput(dt);

	// Call each scene update
	ListItem<Scene*>* item = scenes.start;
	while (item != nullptr )
	{
		if(item->data->active == true)
			item->data->Update(dt);

		item = item->next;
	}

	CheckWin();

	return ret;
}

bool SceneManager::PostUpdate()
{
	ListItem<Scene*>* item = scenes.start;

	while (item != nullptr)
	{
		if (item->data->active == true)
			item->data->Draw();

		item = item->next;
	}

	return true;
}

bool SceneManager::HandleInput(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		app->RequestLoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->RequestSaveGame();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN)
		RestartPlayerPosition();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KeyState::KEY_DOWN)
		app->map->viewHitboxes = !app->map->viewHitboxes;

	if (app->input->GetKey(SDL_SCANCODE_F2) == KeyState::KEY_DOWN)
		app->fade->Fade((Scene*)scene1, (Scene*)scene2, 1 / dt);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(-4);

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(4);

	return ret;
}

bool SceneManager::RestartPlayerPosition()
{
	return true;
}

void SceneManager::CheckWin()
{
	int ret = -1;
	ListItem<Scene*>* item = scenes.start;

	while (item != nullptr)
	{
		if(item->data->active == true)
		{
			item->data->CheckWin();
			break;
		}
		item = item->next;
	}

}

void SceneManager::AddScene(Scene* scene, bool active)
{
	//scene = new Scene();
	
	scene->active = active;
	scenes.Add(scene);

}
