#include "App.h"
#include "Audio.h"
#include "Fonts.h"
#include "Textures.h"
#include "Window.h"
#include "Render.h"
#include "SceneManager.h"
#include "GuiButton.h"


GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    bounds.x /= app->win->GetScale();
    bounds.y /= app->win->GetScale();
    this->bounds = bounds;
    this->text = text;

    texture = app->tex->Load("Assets/Textures/atlas.png");
    fxMouseClick = app->audio->LoadFx("Assets/Audio/Fx/UI/button_release.wav");
   

    highlighted.PushBack({ 2,34,133,26 });
    highlighted.PushBack({ 140,34,133,26 });
    highlighted.PushBack({ 279,34,133,26 });
    highlighted.PushBack({ 421,34,133,26 });
    highlighted.PushBack({ 279,34,133,26 });
    highlighted.PushBack({ 140,34,133,26 });
    highlighted.PushBack({ 2,34,133,26 });
    highlighted.loop = true;

    buttonPressed.PushBack( { 197,102,125,18 });
    buttonPressed.PushBack( { 197,102,125,18 });
    buttonPressed.PushBack( { 197,102,125,18 });
    buttonPressed.loop = false;

    currentAnim = nullptr;
}


GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt, iPoint position)
{
    position.x /= app->win->GetScale();
    position.y /= app->win->GetScale();

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
        mouseX += -(app->render->camera.x) / (int)app->win->GetScale();
        mouseY += -(app->render->camera.y) / (int)app->win->GetScale();

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
                app->audio->PlayFx(fxMouseClick);
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    if (currentAnim != nullptr)
        currentAnim->Update();

    return true;
}

bool GuiButton::Draw(Render* render, bool debugDraw)
{
    if (state != GuiControlState::DISABLED)
    {
        SDL_Rect sect = { 59,102, 125, 18 };
        render->DrawTexture(texture, bounds.x, bounds.y, &sect);
    }
    else
    {
        SDL_Rect sect = { 349, 102, 125, 18 };
        render->DrawTexture(texture, bounds.x, bounds.y, &sect);
    }

    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
        if (debugDraw)
            render->DrawRectangle(bounds, { 100, 100, 100, 200 });
        break;

    case GuiControlState::NORMAL: 
        if (debugDraw)
            render->DrawRectangle(bounds, { 0, 255, 0, 200 });
        break;

    case GuiControlState::FOCUSED: 
        if (debugDraw)
            render->DrawRectangle(bounds, { 255, 255, 0, 200 });
        
        if (currentAnim != &highlighted)
        {
            highlighted.Reset();
            currentAnim = &highlighted;
        }
        render->DrawTexture(texture, bounds.x - 3, bounds.y - 3, &currentAnim->GetCurrentFrame());
        
        break;

    case GuiControlState::PRESSED: 
        if (currentAnim != &buttonPressed)
        {
            buttonPressed.Reset();
            currentAnim = &buttonPressed;
        }
        render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
        
        if (debugDraw)
            render->DrawRectangle(bounds, { 0, 255, 255, 200 });

        break;

    case GuiControlState::SELECTED: 
        if (debugDraw)
            render->DrawRectangle(bounds, { 0, 255, 0, 200 });
        break;

    default:
        break;
    }

    app->fonts->DrawText(bounds.x + 3 + (app->render->camera.x) / app->win->GetScale(), bounds.y + 3 + (app->render->camera.y) / app->win->GetScale(), 0, text.GetString());
   
    return false;
}