#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Module.h"
#include "Player.h"
#include "NPC.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"

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
	GuiControlButton* AudioOff;
	GuiControlButton* AudioOn;
	GuiControlButton* FxOff;
	GuiControlButton* FxOn;
	GuiControlButton* Vsinc;
	GuiControlButton* turnBack;
	GuiControlButton* FullScreen;
	GuiControlButton* FullScreenOff;
	

	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;

	SDL_Texture* IdleStartButton;
	SDL_Texture* IdleContinueButton;
	SDL_Texture* IdleOptionsButton;
	SDL_Texture* IdleExitButton;
	SDL_Texture* IdleVsyncButton;
	SDL_Texture* IdleFullScreenButton;
	SDL_Texture* IdleNormalScreenButton;
	SDL_Texture* IdleBackButton;

	SDL_Texture* ActiveStartButton;
	SDL_Texture* ActiveContinueButton;
	SDL_Texture* ActiveOptionsButton;
	SDL_Texture* ActiveExitButton;
	SDL_Texture* ActiveVsyncButton;
	SDL_Texture* ActiveFullScreenButton;
	SDL_Texture* ActiveNormalScreenButton;
	SDL_Texture* ActiveBackButton;

	bool isExiting = false;

	bool isOnSettings = false;
	int uiGamePadCounter = 0;
	
public:
	const char* texturePath;

	Animation menuAnim;
	Animation* currentAnimation = nullptr;
	bool isFlipped = false;


	unsigned int hoverFx;
	unsigned int clickFx;
	unsigned int declineFx;
	unsigned int deniedFx;

};

#endif // __MAINMENU_H__
