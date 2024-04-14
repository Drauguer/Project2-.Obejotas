#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Module.h"
#include "Player.h"
#include "NPC.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class MainMenu : public Module
{
public:

	MainMenu(bool startEnabled);

	// Destructor
	virtual ~MainMenu();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Enable Module
	void Enable();

	// Disable Module
	void Disable();

	unsigned int TitleScreenMusic;

private:
	SDL_Texture* img;
	GuiControlButton* start;
	GuiControlButton* exit;
	GuiControlButton* setting;
	GuiControlButton* continue_;
	GuiControlButton* VsincOff;
	GuiControlButton* Vsinc;
	GuiControlButton* turnBack;
	GuiControlButton* FullScreen;
	GuiControlButton* FullScreenOff;
	

	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;



	
public:
	
};

#endif // __MAINMENU_H__
