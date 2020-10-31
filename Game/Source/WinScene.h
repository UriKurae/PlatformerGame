#pragma once
#include "Module.h"

#include "SDL_image/include/SDL_image.h"
class SDL_Texture;

class WinScene : public Module
{
public:

	WinScene();

	~WinScene();

	bool Awake(pugi::xml_node&) override;

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

private: 
	SString folder;

	SDL_Texture* bg;
};
