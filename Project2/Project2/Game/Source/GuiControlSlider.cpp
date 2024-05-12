#include "GuiControlSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Window.h"

// Constructor
GuiControlSlider::GuiControlSlider(uint32 id, SDL_Rect bounds, const char* text)
    : GuiControl(GuiControlType::SLIDER, id), value(0.0f), minValue(0.0f), maxValue(1.0f)
{
    this->bounds = bounds;
    this->text = text;

    // Slider specific properties
    sliderButton = { bounds.x+ bounds.x/2, bounds.y, 10, bounds.h }; // Small rectangle representing the slider handle
}

// Destructor
GuiControlSlider::~GuiControlSlider()
{

}

// Update method
bool GuiControlSlider::Update(float dt)
{
    int scale = app->win->GetScale();

    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check if the mouse is over the slider
        if (mouseX * scale > bounds.x && mouseX * scale < bounds.x + bounds.w && mouseY * scale > bounds.y && mouseY * scale < bounds.y + bounds.h) {

            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
                state = GuiControlState::PRESSED;

                // Update the slider value based on mouse position
                newValue = (float)(mouseX - bounds.x*scale) / (float)(bounds.w*scale);
                newValue = minValue + newValue * (maxValue - minValue);
                SetValue(newValue);
                
            }
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
                NotifyObserver();
            }
        }
        else
        {
            state = GuiControlState::NORMAL;
        }

        // Draw slider
        app->render->DrawRectangle(bounds, 255, 255, 255, 255, true, false); // Draw the slider bar
        app->render->DrawRectangle(sliderButton, 0, 255, 0, 255, true, false); // Draw the handle

        // Draw text
        app->render->DrawText(text.GetString(), bounds.x, bounds.y - 50, bounds.w, 50);
    }

    return false;
}

void GuiControlSlider::SetValue(float value)
{
    int scale = app->win->GetScale();
    this->value = value;
    // Update slider button position based on value
    sliderButton.x = bounds.x*scale + (int)((value - minValue) / (maxValue - minValue) * bounds.w*scale);
}

float GuiControlSlider::GetValue() const
{
    return value;
}
