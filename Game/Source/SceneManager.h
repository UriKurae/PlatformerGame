#pragma once

#include "Module.h"
#include "Point.h"
#include "Scene.h"

class SDL_Texture;

class SceneManager : public Module
{
public:

	SceneManager();
	virtual ~SceneManager();

	bool Update(float dt);

	void Draw();

	// Restart level
	bool RestartPlayerPosition();

	// Check if player won
	int CheckWin();

private:

	List<Scene*> scenes;
	
	
};