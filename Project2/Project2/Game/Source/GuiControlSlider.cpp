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
    canClick = true;
    drawBasic = false;
    maxValue = 128;
    minValue = 0;
    minValueFrom = bounds.x;
    maxValueFrom = bounds.x + bounds.w;
}

// Destructor
GuiControlSlider::~GuiControlSlider()
{

}

// Update method
bool GuiControlSlider::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// Update the state of the GUiButton according to the mouse position
		int mouseX, mouseY;
		float screenScale = 1 / (float)app->win->GetScale();

		SDL_GetMouseState(&mouseX, &mouseY);


		if ((mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) ||
			(mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h)) {
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{

				//10* is an offset to get the thumb in the right position
				sliderButton.x = mouseX - (10);

				if (mouseX < bounds.x || sliderButton.x < bounds.x)
					sliderButton.x = bounds.x;

				if (mouseX > (bounds.x + bounds.w) || sliderButton.x + 40 > (bounds.x + bounds.w))
					sliderButton.x = (bounds.x + bounds.w) - 40;


				value = UpdateValue(mouseX);
				//LOG("slider value:%f", GetValue(mouseX));
				state = GuiControlState::PRESSED;
			}
			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}

		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

int GuiControlSlider::UpdateValue(float pos)
{

	if (pos <= bounds.x + (sliderButton.w * 0.5f))
		return value = minValue;

	if (pos >= (bounds.x + bounds.w) + (sliderButton.w * 0.5f))
		return	value = maxValue;

	value = minValue + (maxValue - minValue) * ((pos - minValueFrom) / (maxValueFrom - minValueFrom));

	return value;
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
