#ifndef __GUICONTROLBUTTON_H__
#define __GUICONTROLBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* texture);
	virtual ~GuiControlButton();

	// Called each loop iteration
	bool Update(float dt);

	bool isButtonPressed = false;

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
	int cooldown = 0;
};

#endif // __GUICONTROLBUTTON_H__