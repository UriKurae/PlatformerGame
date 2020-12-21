#pragma once

#include "Module.h"
#include "Point.h"
#include "Scene.h"
#include "Animation.h"

class SDL_Texture;
class SceneLogo;
class MainMenu;
class Scene1;
class Scene2;

class SceneManager : public Module
{
public:

	SceneManager();
	virtual ~SceneManager();
	
	bool Start();

	bool Update(float dt);

	// Draw everything from the current scene
	bool PostUpdate();

	bool Load(pugi::xml_node&) override;

	bool Save(pugi::xml_node&) override;

	// Is bool because we want to detect when ESC is pressed
	bool HandleInput(float dt);

	// Check if player won
	void CheckWin();

	void AddScene(Scene* scene, bool active);

	void ChangeScene(Scene* scene);

public:

	bool loadedOnScene = false;

	List<Scene*> scenes;
	Scene* lastScene;
	Scene* currentScene;
	Scene* nextScene;
	Scene* savedScene = nullptr;

	SceneLogo* sceneLogo;
	MainMenu* mainMenu;
	Scene1* scene1;
	Scene2* scene2;
	DeadScene* deadScene;
	WinScene* winScene;

	SDL_Texture* gemTexture;
	SDL_Texture* checkpointTexture;

	// Animations for checkPoints
	Animation checkpointAnim;
	Animation checkpointKeepAnim;

	int checkpointFx;

	// Lists for enemies saved positions
	List<iPoint> wolfSavedPositions;
	List<iPoint> executionerSavedPositions;


	bool onTransition = false;
	bool fadeOutCompleted = false;
	float transitionAlpha;
};