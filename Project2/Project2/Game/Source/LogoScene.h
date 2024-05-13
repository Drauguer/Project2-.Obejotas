#pragma once




#include "Module.h"
#include "Player.h"
#include "NPC.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class LogoScene : public Module
{
public:

	LogoScene(bool startEnabled);

	// Destructor
	virtual ~LogoScene();

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

	unsigned int logoScreenMusic;
	const char* texturePath;

	Animation logoAnim;
	Animation* currentAnimation = nullptr;
	bool isFlipped = false;


private:
	SDL_Texture* img;
	float timer;


	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;




public:

	bool fadeTriggered = false;

};