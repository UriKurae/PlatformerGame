#pragma once

#include "Module.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "Point.h"
#include "Scene.h"
#include "Animation.h"

class SDL_Texture;
class SceneLogo;
class MainMenu;
class Scene1;
class Scene2;


enum class MenuState
{
	NONE = -1,
	INITIAL,
	OPTIONS,
	CREDITS,
	LICENSE,
};

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

	void ShowPauseMenu();

	int CalculateFinalScore();

public:

	bool isPaused = false;

	bool loadedOnScene = false;

	// Check if theres a current game saved
	pugi::xml_document saveAvailableDocument;
	bool saveAvailable = false;
	bool newGame = true;

	List<Scene*> scenes;
	Scene* lastScene = nullptr;
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
	Scene* savedScene = nullptr;

	SceneLogo* sceneLogo = nullptr;
	MainMenu* mainMenu = nullptr;
	Scene1* scene1 = nullptr;
	Scene2* scene2 = nullptr;
	DeadScene* deadScene = nullptr;
	WinScene* winScene = nullptr;

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

	MenuState statusMenu;

	// GUI Buttons In-Game
	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnBackToTitle;
	GuiButton* btnExit;

	// Options GUI
	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFxVolume;
	GuiCheckBox* fullScreenCheckBox;
	GuiCheckBox* vSyncCheckBox;
	GuiButton* btnBackOptions;

	int musicVolume = 25;
	int fxVolume = 25;

	int uiIndex;
	SDL_Texture* timerTexture;

	// Player score
	int score = 0;
	int wolvesKilled = 0;
	int executionersKilled = 0;
	int gemsPicked = 0;
	int heartsPicked = 0;
};