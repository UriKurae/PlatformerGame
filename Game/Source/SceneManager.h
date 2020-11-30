#pragma once

#include "Module.h"
#include "Point.h"
#include "Scene.h"

class SDL_Texture;
class Scene1;
class Scene2;

enum SwitchStep
{
	NONE,
	TO_BLACK,
	FROM_BLACK
};

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

	void SwitchScene(Scene* toDisable, Scene* toEnable, float frames);

	void CheckSwitchState();

	void AddScene(Scene* scene);

private:
	
	List<Scene*> scenes;
	SwitchStep currentStep = SwitchStep::NONE;
	
	Scene1* scene1;
	Scene2* scene2;

	// Switching stuff
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;

	SDL_Rect screenRect;

	Scene* sceneToEnable = nullptr;
	Scene* sceneToDisable = nullptr;


};