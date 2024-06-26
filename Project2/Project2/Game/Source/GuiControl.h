#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Module.h"

#include "Point.h"
#include "SString.h"


#include "SDL/include/SDL.h"
#include "Log.h"

enum class GuiControlType
{
	BUTTON,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER
};

enum class GuiControlState
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};
enum class FunctionGUI
{
	START,
	CONTINUE,
	EXIT,
	SETTINGS,
	CREDITS,
	MUSIC,
	FX,
	FULLSCREEN,
	VSYNC,
	TIMER,
	LIVES,
	COINS,

	RESUME,
	BACKTOTITLE,
	BACKTOPAUSE
};

class GuiControl
{
public:

	// Constructor
	GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL) {}

	// Constructor
	GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) :
		type(type),
		state(GuiControlState::NORMAL),
		bounds(bounds),
		text(text)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// 
	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
		section = { 0, 0, 0, 0 };
	}

	// 
	void SetObserver(Module* module)
	{
		observer = module;
	}

	void SetFunction(FunctionGUI function)
	{
		this->function = function;
	}

	// 
	void NotifyObserver()
	{

		observer->OnGuiMouseClickEvent(this);
	}

public:

	uint32 id;
	GuiControlType type;
	GuiControlState state;
	FunctionGUI function;

	SString text;           // Control text (if required)
	SDL_Rect bounds;        // Position and size
	SDL_Color color;        // Tint color

	SDL_Texture* texture;   // Texture atlas reference
	SDL_Rect section;       // Texture atlas base section

	Module* observer;        // Observer 



	//checking
	bool hasToExit;
};

#endif // __GUICONTROL_H__