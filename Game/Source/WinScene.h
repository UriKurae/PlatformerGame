#pragma once

#include "Module.h"

struct SDL_Texture;

class WinScene : public Scene
{
public:

	WinScene();

	// Destructor
	virtual ~WinScene();

	// Called before the first frame
	bool Start() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool Draw() override;

	// Called before quitting
	bool CleanUp() override;

	bool OnGuiMouseClickEvent(GuiControl* control);

	void DrawScore();

private:

	SDL_Texture* winTexture = nullptr;

	int score = 0;

	char wolvesText[4] = { "\0" };
	char executionerText[4] = { "\0" };
	char gemsText[4] = { "\0" };
	char heartText[4] = { "\0" };
	char finalScoreText[10] = { "\0" };


	GuiButton* btnBackToMenu;
	GuiButton* btnSeeScore;
	GuiButton* btnBackSeeScore;

	bool showScore;
};