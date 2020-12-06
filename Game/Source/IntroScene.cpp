#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "IntroScene.h"
#include "SceneManager.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"


IntroScene::IntroScene()
{
	name.Create("introScene");
	count = 200;
}

IntroScene::~IntroScene()
{

}

// Load assets
bool IntroScene::Start()
{
	intro = app->tex->Load("Assets/Textures/Scenes/intro.png");
	logo = app->tex->Load("Assets/Textures/Scenes/logo.png");
	
	app->audio->PlayMusic("Assets/Audio/Music/intro_scene.ogg");

	app->render->SetCameraPosition(0,0);

	return true;
}

bool IntroScene::Update(float dt)
{
	if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) && (count <= 0) && (showLogo == false))
		app->fade->Fade(this, (Scene*)app->sceneManager->scene1, 1 / dt);
	
	if ((count >= 100) && (count <= 110))
	{
		app->fade->Fade(this, this, 1 / dt);
	}
	else if (count <= 40)
	{
		showLogo = false;
	}
	
	count -= 20 * dt;

	return true;
}

// Update: draw background
bool IntroScene::Draw()
{
	bool ret = true;

	if ((count >= 65) && (showLogo == true))
	{
		app->render->DrawTexture(logo, 150, 0, NULL);
	}
	else if (count <= 60)
	{
		app->render->DrawTexture(intro, 0, 0, NULL);
	}
	return ret;
}

bool IntroScene::CleanUp()
{
	app->tex->UnLoad(intro);
	app->tex->UnLoad(logo);
	
	return true;
}