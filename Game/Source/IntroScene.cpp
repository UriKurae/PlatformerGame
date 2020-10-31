
#include "App.h"
#include "IntroScene.h"

#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"


IntroScene::IntroScene()
{
	name.Create("introscene");

}

IntroScene::~IntroScene()
{

}

bool IntroScene::Awake(pugi::xml_node& node)
{
	folder.Create(node.child("folder").child_value());

	return true;
}

// Load assets
bool IntroScene::Start()
{
	intro = app->tex->LoadTexture("intro.png");
	
	app->render->SetCameraPosition(0,0);

	return true;
}

bool IntroScene::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) 
	{ 
		app->fade->FadingToBlack(this, app->scene, 30.0f);
	}
	
	return true;
}

// Update: draw background
bool IntroScene::PostUpdate()
{
	app->render->DrawTexture(intro,0,0,NULL);
	return true;
}

bool IntroScene::CleanUp()
{

	return true;
}