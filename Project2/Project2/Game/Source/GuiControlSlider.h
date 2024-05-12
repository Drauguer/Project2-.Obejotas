#ifndef __GUICONTROLSLIDER_H__
#define __GUICONTROLSLIDER_H__

#include "GuiControl.h"
#include "SDL/include/SDL.h"

#include "Point.h"
#include "SString.h"


class GuiControlSlider : public GuiControl
{
public:
    GuiControlSlider(uint32 id, SDL_Rect bounds, const char* text);

    virtual ~GuiControlSlider();

    bool Update(float dt) override;
    // You can also override the Render method if needed

    void SetValue(float value);
    float GetValue() const;


public:
    float newValue;
protected:
    float value;        // Current value of the slider
    float minValue;     // Minimum value of the slider
    float maxValue;     // Maximum value of the slider

    SDL_Rect sliderButton; // Rectangle representing the slider handle

    // Other properties and methods specific to the slider control...
};

#endif // __GUICONTROLSLIDER_H__
