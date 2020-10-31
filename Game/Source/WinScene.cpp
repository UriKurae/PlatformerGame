#include "WinScene.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"

WinScene::WinScene() : Module()
{
	name.Create("winscene");
}


WinScene::~WinScene()
{
}

bool WinScene::Awake(pugi::xml_node& config)
{
	folder.Create(config.child("folder").child_value());

	return true;
}

bool WinScene::Start()
{
	/*SString tmp("%s%s", folder.GetString(), "win.png");
	bg = app->tex->Load(tmp.GetString());*/


	return true;
}

bool WinScene::Update(float dt)
{
	return true;
}

bool WinScene::PostUpdate()
{
	app->render->DrawTexture(bg, 0, 0, NULL);
	return true;
}

bool WinScene::CleanUp()
{
	app->tex->UnLoad(bg);
	return true;
}
