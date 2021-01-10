#include "App.h"
#include "Audio.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Fonts.h"
#include "SceneManager.h"
#include "GuiSlider.h"

#include "Log.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	if (id == 1)
	{
		this->value = app->audio->GetMusicVolume();
	}
	else if (id == 2)
	{
		this->value = app->audio->GetFxVolume();
	}

	bounds.x = this->value;
	bounds.y /= app->win->GetScale();

	this->bounds = bounds;
	this->bounds.h = bounds.h + 1;
	this->text = text;

	if (app->sceneManager->currentScene == (Scene*)app->sceneManager->mainMenu)
	{
		this->minValue = 265;
		this->maxValue = 365;
	}
	else
	{
		this->minValue = bounds.x - value;
		this->maxValue = bounds.x + bounds.w + (100 - value);
	}
	

	texture = app->tex->Load("Assets/Textures/Gui/atlas.png");
	fxMouseRelease = app->audio->LoadFx("Assets/Audio/Fx/UI/slider_release.wav");

	fxMouseClick = app->audio->LoadFx("Assets/Audio/Fx/UI/button_release.wav");
	fxMouseHover = app->audio->LoadFx("Assets/Audio/Fx/UI/button_hover.wav");
	mouseFxDone = false;
	
	CalculateValue();
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt, iPoint position)
{
	position.x /= (int)app->win->GetScale();
	position.y /= (int)app->win->GetScale();

	if (id == 1)
	{
		this->value = app->audio->GetMusicVolume();
	}
	else if (id == 2)
	{
		this->value = app->audio->GetFxVolume();
	}

	if (initialPos.x != position.x || initialPos.y != position.y)
	{
		if (app->sceneManager->currentScene == (Scene*)app->sceneManager->mainMenu)
		{
			bounds.x = position.x;
			this->minValue = 265;
			this->maxValue = 365;
		}  
		else
		{
			bounds.x = position.x + (this->value);
			this->minValue = bounds.x - (this->value);
			this->maxValue = bounds.x + (100 - this->value);
		}

		initialPos.x = position.x;
		initialPos.y = position.y;
		bounds.y = position.y;
	}

	if (state != GuiControlState::DISABLED)
	{
		input->GetMousePosition(mouseX, mouseY);

		// Camera offset applied to the mouse so we can use the options.
		mouseX += -app->render->camera.x / (int)app->win->GetScale();
		mouseY += -app->render->camera.y / (int)app->win->GetScale();

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)) && (state != GuiControlState::PRESSED))
		{
			if (mouseFxDone == false)
			{
				app->audio->PlayFx(fxMouseHover);
				mouseFxDone = true;
			}
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
				state = GuiControlState::PRESSED;

		}
		else if (state == GuiControlState::PRESSED)
		{
			if ((input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) && (mouseX > minValue + 3) && (mouseX < maxValue + 3))
			{
				bounds.x = ((mouseX) - ((bounds.w / 2) / (int)app->win->GetScale()));
				CalculateValue();
				NotifyObserver();
			}
			else if ((input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) && (id == 2))
			{
				app->audio->PlayFx(fxMouseRelease);
			}
			else
			{
				state = GuiControlState::NORMAL;
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
			mouseFxDone = false;
		}
	}

	return true;
}

bool GuiSlider::Draw(Render* render, bool debugDraw)
{
	// Box
	SDL_Rect sect = { 110,134,110,16 };
	if (app->sceneManager->currentScene == (Scene*)app->sceneManager->mainMenu)
	{
		render->DrawTexture(texture, 265, bounds.y, &sect);
	}
	else
	{
		render->DrawTexture(texture, minValue, bounds.y, &sect);
	}

	// Sword
	sect = { 25,170,6,19 };
	render->DrawTexture(texture, bounds.x, bounds.y - 5, &sect);

	// Draw the slider depending on state
	switch (state)
	{
	case GuiControlState::DISABLED: 
		if(debugDraw)
			render->DrawRectangle(bounds, { 100, 100, 100, 200 });
		break;

	case GuiControlState::NORMAL: 
		if(debugDraw)
			render->DrawRectangle(bounds, { 0, 0, 0, 200 });
		break;

	case GuiControlState::FOCUSED: 
		sect = { 23,197,9,21 };
		render->DrawTexture(texture, ((bounds.x) - (bounds.w / 2)), bounds.y - 5, &sect);

		if (debugDraw)
			render->DrawRectangle(bounds, { 255, 255, 0, 200 });

		break;

	case GuiControlState::PRESSED:
		sect = { 67, 196, 9,21 };
		render->DrawTexture(texture, (bounds.x - (bounds.w / 2)), bounds.y - 5, &sect);
		
		if (debugDraw)
			render->DrawRectangle(bounds, { 0, 255, 255, 255 });
		
		break;

	case GuiControlState::SELECTED: 
		if(debugDraw)
			render->DrawRectangle(bounds, { 0, 255, 0, 200 });
		break;

	default:
		break;
	}


	app->fonts->DrawText(minValue - 20 + (app->render->camera.x) / (int)app->win->GetScale(), bounds.y - 15 + (app->render->camera.y) / (int)app->win->GetScale(), 0, text.GetString());

	return false;
}

void GuiSlider::CalculateValue()
{
	/*this->value = bounds.x - minValue;

	if (this->value > 100)
		this->value = 100;
	
	else if (this->value <= 3)
		this->value = 0;*/

	this->value = bounds.x - minValue;

	if (this->value > 100)
		this->value = 100;

	else if (this->value <= 3)
		this->value = 0;

	if (id == 1)
	{
		app->audio->SetMusicVolume(this->value);
	}
	else if (id == 2)
	{
		app->audio->SetFxVolume(this->value);
	}


	LOG("Value of the thing %i", value);
}

int GuiSlider::GetValue() const
{
	return this->value;
}
