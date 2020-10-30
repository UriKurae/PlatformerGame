#pragma once

#include "Module.h"

struct SDL_Texture;

class IntroScene : public Module
{
public:
	//Constructor
	IntroScene();

	//Destructor
	~IntroScene();

	// Called when the module is activated
	bool Awake(pugi::xml_node& config) override;


	// Loads the necessary textures for the map background
	bool Start() override;

	// Called before render is available

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool CleanUp() override;

private:

	SDL_Texture* intro;
	

};

