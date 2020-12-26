#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Render.h"
#include "SceneManager.h"
#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
    texture = app->tex->Load("Assets/Textures/atlas.png");

    highlighted.PushBack({ 5,38,66,21 });
    highlighted.PushBack({ 78,38,66,21 });
    highlighted.PushBack({ 151,38,66,21 });
    highlighted.PushBack({ 227,38,66,21 });
    highlighted.PushBack({ 151,38,66,21 });
    highlighted.PushBack({ 78,38,66,21 });
    highlighted.PushBack({ 5,38,66,21 });
    highlighted.loop = true;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt, iPoint position)
{
    highlighted.speed = 10.0f * dt;

    if (bounds.x != position.x || bounds.y != position.y)
    {
        bounds.x = position.x;
        bounds.y = position.y;
    }

    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);
        
        // Camera offset applied to the mouse so we can use the options.
        mouseX += -app->render->camera.x / app->win->GetScale();
        mouseY += -app->render->camera.y / app->win->GetScale();

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    if (currentAnim != nullptr)
        currentAnim->Update();

    return false;
}

bool GuiButton::Draw(Render* render)
{
    SDL_Rect sect = { 59,106,60,15 };
    render->DrawTexture(texture, bounds.x, bounds.y, &sect);

    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 100, 100, 100, 255 });
        break;
    case GuiControlState::NORMAL: //render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    case GuiControlState::FOCUSED: 
        //render->DrawRectangle(bounds, { 255, 255, 0, 255 });
        if (currentAnim != &highlighted)
        {
            highlighted.Reset();
            currentAnim = &highlighted;
        }
        render->DrawTexture(texture, bounds.x - 3, bounds.y - 3, &highlighted.GetCurrentFrame());
        break;
    case GuiControlState::PRESSED: render->DrawRectangle(bounds, { 0, 255, 255, 255 });
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    default:
        break;
    }

    return false;
}