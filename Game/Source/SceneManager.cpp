#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Map.h"
#include "Player.h"
#include "SceneManager.h"
#include "IntroScene.h"
#include "Scene1.h"
#include "Scene2.h"
#include "DeadScene.h"
#include "WinScene.h"
#include "FadeToBlack.h"
#include "EnemyManager.h"


#include "SDL/include/SDL_scancode.h"

SceneManager::SceneManager()
{
	name.Create("scenemanager");

	introScene = new IntroScene();
	scene1 = new Scene1();
	scene2 = new Scene2();
	deadScene = new DeadScene();
	winScene = new WinScene();

	AddScene(introScene, false);
	AddScene(scene1, true);
	AddScene(scene2, false);
	AddScene(deadScene, false);
	AddScene(winScene, false);


	checkpointAnim.PushBack({0,0,11,9});
	checkpointAnim.PushBack({44,0,12,19});
	checkpointAnim.PushBack({90,0,15,31});
	checkpointAnim.PushBack({135,0,17,51});
	checkpointAnim.PushBack({180,0,16,90});
	checkpointAnim.PushBack({225,0,17,49});
	checkpointAnim.PushBack({270,0,17,49});
	checkpointAnim.PushBack({315,0,17,49});
	checkpointAnim.PushBack({360,0,17,35});
	checkpointAnim.loop = false;

	checkpointKeepAnim.PushBack({ 89,28,17,51 });
	checkpointKeepAnim.PushBack({ 120,29,16,50 });
	checkpointKeepAnim.PushBack({ 149,30,17,49 });
	checkpointKeepAnim.PushBack({ 180,31,17,49 });
	checkpointKeepAnim.PushBack({ 210,29,17,50 });
	checkpointKeepAnim.PushBack({ 242,26,17,53 });
	checkpointKeepAnim.loop = true;
}

SceneManager::~SceneManager()
{
	scenes.Clear();
}

bool SceneManager::Load(pugi::xml_node& node)
{
	int count = 0;
	count = node.child("active_scene").attribute("value").as_int();

	if (count == 1)
		savedScene = scene1;

	else if (count == 2)
		savedScene = scene2;

	ListItem<Enemy*>* item = app->enemyManager->enemies.start;
	pugi::xml_node enemies = node.child("enemies");
	pugi::xml_node wolf = enemies.child("wolf");
	pugi::xml_node executioner = enemies.child("executioner");

	while (item != nullptr)
	{
		if (item->data->name == "wolf")
		{
			if(item->data->savedIsAlive)
				item->data->Load(wolf);

			wolf = wolf.next_sibling("wolf");
		}
		else if (item->data->name == "executioner")
		{
			if(item->data->savedIsAlive)
				item->data->Load(executioner);

			executioner = executioner.next_sibling("executioner");
		}
		item = item->next;
	}

	return true;
}

bool SceneManager::Save(pugi::xml_node& node)
{
	ListItem<Scene*>* item = scenes.start;
	int count = 0;
	while (item != nullptr)
	{
		if (item->data->active == true)
			break;

		item = item->next;
		++count;
	}

	node.append_child("active_scene").append_attribute("value").set_value(count);
	
	ListItem<Enemy*>* it = app->enemyManager->enemies.start;
	pugi::xml_node enemies = node.append_child("enemies");
	while (it != nullptr)
	{
		it->data->Save(enemies.append_child(it->data->name.GetString()));
		it = it->next;
	}

	return true;


	return true;
}


bool SceneManager::Start()
{
	checkpointTexture = app->tex->Load("Assets/Textures/Scenes/checkpoint.png");
	checkpointFx = app->audio->LoadFx("Assets/Audio/Fx/checkpoint.wav");

	// Call all Scenes' start
	ListItem<Scene*>* item = scenes.start;

	while (item != nullptr)
	{
		if (item->data->active == true)
			item->data->Start();

		item = item->next;
	}
	
	return true;
}

bool SceneManager::Update(float dt)
{
	bool ret = true;

	checkpointAnim.speed = 2.0f * dt;
	checkpointKeepAnim.speed = 2.0f * dt;

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

	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->RequestSaveGame();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
	{
		app->RequestLoadGame();

		if ((savedScene != nullptr) && (savedScene != currentScene))
		{
			app->player->Disable();
			app->fade->Fade(currentScene, savedScene, 1 / dt);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_O) == KeyState::KEY_DOWN)
		app->fade->Fade((Scene*)scene2, (Scene*)scene1, 1 / dt);

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->fade->Fade(currentScene, scene1, 1 / dt);
	
	if (app->input->GetKey(SDL_SCANCODE_F2) == KeyState::KEY_DOWN)
		app->fade->Fade(currentScene, (Scene*)scene2, 1 / dt);

	if (app->input->GetKey(SDL_SCANCODE_F9) == KeyState::KEY_DOWN)
		app->map->viewHitboxes = !app->map->viewHitboxes;

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
