#include "GuiSlider.h"
#include "Log.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    this->minValue = bounds.x - 25;
    this->maxValue = bounds.x + bounds.w + 25;
    CalculateValue();
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
    input->GetMousePosition(pos.x, pos.y);

    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)) && (state != GuiControlState::PRESSED))
        {
            state = GuiControlState::FOCUSED;
            // TODO.
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
                state = GuiControlState::PRESSED;

        }
        else if (state == GuiControlState::PRESSED)
        {
            if ((input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) && (mouseX > minValue) && (mouseX < maxValue))
            {
                bounds.x = ((pos.x) - (bounds.w / 2));
                CalculateValue();
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
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 100, 100, 100, 255 });
        break;
    case GuiControlState::NORMAL: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 255, 255, 0, 255 });
        break;
    case GuiControlState::PRESSED: render->DrawRectangle({((pos.x) - (bounds.w / 2)), bounds.y, bounds.w, bounds.h}, { 0, 255, 255, 255 });
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
