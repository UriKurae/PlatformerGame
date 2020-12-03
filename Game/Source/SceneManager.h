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

	// Draw everything from the current scene
	bool PostUpdate();

	// Is bool because we want to detect when ESC is pressed
	bool HandleInput(float dt);

	// Restart level
	bool RestartPlayerPosition();

	// Check if player won
	void CheckWin();

	void AddScene(Scene* scene, bool active);

public:

	List<Scene*> scenes;
	Scene* lastScene;

	IntroScene* introScene;
	Scene1* scene1;
	Scene2* scene2;
	DeadScene* deadScene;

};