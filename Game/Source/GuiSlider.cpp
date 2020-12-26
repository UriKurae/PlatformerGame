#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Render.h"
#include "SceneManager.h"
#include "GuiSlider.h"

#include "Log.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    this->minValue = bounds.x - 25;
    this->maxValue = bounds.x + bounds.w + 25;
    texture = app->tex->Load("Assets/Textures/atlas.png");
    CalculateValue();
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt, iPoint position)
{
    if (initialPos.x != position.x || initialPos.y != position.y)
    {
        if (app->sceneManager->currentScene == (Scene*)app->sceneManager->mainMenu)
            bounds.x = position.x;
        else
            bounds.x = position.x + (this->value / app->win->GetScale());

        initialPos.x = position.x;
        initialPos.y = position.y;
        bounds.y = position.y;
        this->minValue = bounds.x - (this->value / app->win->GetScale());
        this->maxValue = bounds.x + ((100 - this->value) / app->win->GetScale());
    }


    CalculateValue();

    if (state != GuiControlState::DISABLED)
    {
        input->GetMousePosition(mouseX, mouseY);

        // Camera offset applied to the mouse so we can use the options.
        mouseX += -app->render->camera.x / app->win->GetScale();
        mouseY += -app->render->camera.y / app->win->GetScale();

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)) && (state != GuiControlState::PRESSED))
        {
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
                state = GuiControlState::PRESSED;

        }
        else if (state == GuiControlState::PRESSED)
        {
            if ((input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) && (mouseX > minValue) && (mouseX < maxValue))
            {
                bounds.x = ((mouseX) - ((bounds.w / 2) / app->win->GetScale()));
                CalculateValue();
                NotifyObserver();
            }
            else
            {
                state = GuiControlState::NORMAL;
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::Draw(Render* render)
{
    SDL_Rect sect = {6,106,50,13};
    render->DrawTexture(texture, minValue, bounds.y - 1, &sect);
    sect = { 6,87,5,10 };
    render->DrawTexture(texture, bounds.x, bounds.y, &sect);
    // Draw the slider depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 100, 100, 100, 255 });
        break;
    case GuiControlState::NORMAL: //render->DrawRectangle(bounds, { 0, 0, 0, 255 });
        break;
    case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 255, 255, 0, 255 });
        break;
    case GuiControlState::PRESSED: //render->DrawRectangle({(bounds.x) - (bounds.w / 2), bounds.y, bounds.w, bounds.h}, { 0, 255, 255, 255 });
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    default:
        break;
    }

    return false;
}

void GuiSlider::CalculateValue()
{
    this->value = (bounds.x - minValue) * 2;

    if (this->value > 100)
        this->value = 100;
    
    else if (this->value < 0)
        this->value = 0;

    LOG("Value of the thing %i", value);
}

int GuiSlider::GetValue() const
{
    return this->value;
}
