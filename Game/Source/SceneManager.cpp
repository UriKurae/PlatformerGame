#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Map.h"
#include "SceneManager.h"
#include "FadeToBlack.h"
#include "Scene1.h"


#include "SDL/include/SDL_scancode.h"

SceneManager::SceneManager()
{
	name.Create("scenemanager");

	scene1 = new Scene1();
	//scene2 = new Scene2();
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	
	
	return true;
}

bool SceneManager::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		app->RequestLoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->RequestSaveGame();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN)
		RestartPlayerPosition();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KeyState::KEY_DOWN)
		app->map->viewHitboxes = !app->map->viewHitboxes;

	if (app->input->GetKey(SDL_SCANCODE_F2) == KeyState::KEY_DOWN)
		//Fade(this, (Module*)app->scene2, 1 / dt);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(-4);

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(4);

	// Call each scene update
	ListItem<Scene*>* item = scenes.start;
	while (item != nullptr && ret)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	return ret;
}

void SceneManager::Draw()
{

	ListItem<Scene*>* item = scenes.start;

	while (item != nullptr)
	{

		if (item->data->active == true)
			item->data->Draw();
		
		item = item->next;
	}

}

bool SceneManager::RestartPlayerPosition()
{
	return true;
}

int SceneManager::CheckWin()
{
	return 0;
}

/*void SceneManager::SwitchScene(Scene* toDisable, Scene* toEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == SwitchStep::NONE)
	{
		currentStep = SwitchStep::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;


		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}
	return ret;

}

/*void SceneManager::CheckSwitchState()
{
	// Exit this function if we are not performing a fade
	if (currentStep == SwitchStep::NONE) return;

	if (currentStep == SwitchStep::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			sceneToDisable->Disable();
			sceneToEnable->Enable();

			currentStep = SwitchStep::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = SwitchStep::NONE;
		}
	}

}
*/
void SceneManager::AddScene(Scene* scene)
{
	scene = new Scene();

	scenes.Add(scene);
}
