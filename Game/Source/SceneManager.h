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

	// Is bool because we want to detect when ESC is pressed
	bool HandleInput(float dt);

	// Restart level
	bool RestartPlayerPosition();

	// Check if player won
	int CheckWin();

	void AddScene(Scene* scene, bool active);

public:
	float delt;
	List<Scene*> scenes;

	Scene1* scene1;
	Scene2* scene2;

};