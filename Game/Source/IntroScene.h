#pragma once

#include "App.h"

class IntroScene : public Module
{
public:
	//Constructor
	IntroScene();

	//Destructor
	~IntroScene();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool CleanUp() override;

public:
	

};

