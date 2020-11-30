#include "App.h"
#include "Audio.h"
#include "Input.h"
//#include "Map.h"
#include "SceneManager.h"
#include "FadeToBlack.h"

#include "SDL/include/SDL_scancode.h"

SceneManager::SceneManager()
{
	name.Create("scenemanager");
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		app->RequestLoadGame();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->RequestSaveGame();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN)
		RestartPlayerPosition();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KeyState::KEY_DOWN)
	//TOFIX	app->map->viewHitboxes = !app->map->viewHitboxes;

	if (app->input->GetKey(SDL_SCANCODE_F2) == KeyState::KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene2, 1 / dt);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(-4);

	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KeyState::KEY_DOWN)
		app->audio->VolumeControl(4);

	return true;
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
