#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;
struct Executioner;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config) override;

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Restart level
	bool RestartPlayerPosition();

	// Check if player won
	int CheckWin();
	
private:

	//Point<float> playerStartPosition;
	SDL_Texture* sky;
	SDL_Texture* clouds;
	SDL_Texture* sea;

	Executioner* executioner;
};

#endif // __SCENE_H__