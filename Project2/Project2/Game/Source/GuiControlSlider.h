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
    int UpdateValue(float pos);
    float GetValue() const;


public:
    float newValue;
protected:
    bool canClick = true;
    bool drawBasic = false;
    float value;        // Current value of the slider
    float minValue= 0.0f;     // Minimum value of the slider
    float maxValue=100;     // Maximum value of the slider
    int maxValueFrom, minValueFrom;
    SDL_Rect sliderButton; // Rectangle representing the slider handle

    // Other properties and methods specific to the slider control...
};

#endif // __GUICONTROLSLIDER_H__
