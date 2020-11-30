#pragma once

#include "Module.h"
#include "Point.h"
#include "Scene.h"

class SDL_Texture;
class Scene1;
class Scene2;
class FadeToBlack;

class SceneManager : public Module
{
public:

	SceneManager();
	virtual ~SceneManager();
	
	bool Start();

	bool Update(float dt);

	void Draw();

	// Restart level
	bool RestartPlayerPosition();

	// Check if player won
	int CheckWin();

	void AddScene(Scene* scene);

private:
	
	List<Scene*> scenes;

	Scene1* scene1;
	Scene2* scene2;
	
	FadeToBlack* fader = nullptr;
};