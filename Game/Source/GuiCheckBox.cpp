#include "App.h"
#include "Audio.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Fonts.h"
#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    bounds.x /= app->win->GetScale();
    bounds.y /= app->win->GetScale();
    this->bounds = bounds;
    this->text = text;
    texture = app->tex->Load("Assets/Textures/Gui/atlas.png");
    fxMouseClick = app->audio->LoadFx("Assets/Audio/Fx/UI/check_box_release.wav");
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt, iPoint position)
{
    position.x /= app->win->GetScale();
    position.y /= app->win->GetScale();

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
                app->audio->PlayFx(fxMouseClick);
                checked = !checked;
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return true;
}

bool GuiCheckBox::Draw(Render* render, bool debugDraw)
{
    app->fonts->DrawText(bounds.x - 110 + (app->render->camera.x) / app->win->GetScale(), bounds.y + (app->render->camera.y) / app->win->GetScale(), 0, text.GetString());
   
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
    {
        if (debugDraw)
        {
            if (checked) render->DrawRectangle(bounds, { 100, 100, 100, 255 });
            else render->DrawRectangle(bounds, { 100, 100, 100, 255 });
        }
    }
    break;

    case GuiControlState::NORMAL: 
    {
        if (checked)
        {
            SDL_Rect sect = { 42,133,17,17 };
            render->DrawTexture(texture, bounds.x, bounds.y, &sect);

            if (debugDraw)
                render->DrawRectangle(bounds, { 0, 255, 0, 200 }); // Green
        }
        else
        {
            SDL_Rect sect = { 15,134,17,17 };
            render->DrawTexture(texture, bounds.x, bounds.y, &sect);

            if (debugDraw)
                render->DrawRectangle(bounds, { 255, 0, 0, 200 }); // Red
        }
    } 
    break;

    case GuiControlState::FOCUSED: 
        if (checked)
        {
            SDL_Rect sect = { 94,165,17,17 };
            render->DrawTexture(texture, bounds.x, bounds.y, &sect);
        }
        else
        {
            SDL_Rect sect = { 121,165,17,17 };
            render->DrawTexture(texture, bounds.x, bounds.y, &sect);
        }
        
        if (debugDraw)
        {
            render->DrawRectangle(bounds, { 255, 255, 0, 200 }); 
        }
    break;

    case GuiControlState::PRESSED: 
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

    return false;
}
