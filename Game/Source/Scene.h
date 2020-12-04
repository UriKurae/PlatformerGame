#pragma once

#include "Point.h"
#include "SString.h"

class SDL_Texture;

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

public:

	SString name;
	bool active;

	bool deadOnScene = false;

};