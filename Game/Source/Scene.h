#pragma once

#include "Point.h"
#include "SString.h"

class SDL_Texture;
class GuiControl;
class Item;
class SDL_Rect;

class Scene
{
public:

	Scene() {};
	virtual ~Scene() {};

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Draw Method
	virtual bool Draw()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// Restart level
	virtual bool RestartPlayerPosition()
	{
		return true;
	}

	// Check if player won
	virtual int CheckWin()
	{
		return -1;
	}

	virtual bool CheckCollisions(SDL_Rect& a, SDL_Rect& b)
	{
		return true;
	}

	void EnableScene()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void DisableScene()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	void TransitionToScene(Scene* scene)
	{
		transitionRequired = true;
		nextScene = scene;
	}

	// Define multiple Gui Event methods
	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:
	
	// Draw Gui rectangles
	bool guiDebugDraw;

	SString name;
	bool active;
	int uiIndex;

	bool deadOnScene = false;

	bool transitionRequired;
	Scene* nextScene;

	// Indicates when the exit button is pressed
	bool toExit = false;
};