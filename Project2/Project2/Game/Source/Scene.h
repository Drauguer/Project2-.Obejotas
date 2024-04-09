#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "NPC.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

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

	// Return the player position
	iPoint GetPLayerPosition();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Enable Module
	void Enable();
	
	// Disable Module
	void Disable();

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;

	

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;

	// Test Dialogue
	Dialogue* testDialogue;
	Dialogue* testDialogue2;

public:
	//L03: DONE 3b: Declare a Player attribute
	Player* player;
	List<Player*> allies;


	//List of enemies (reemplazar porque no son players xd) 
	List<Entity*> enemies;


	bool isOnCombat = false;

	//Fullscreen
	bool isFullScreen = false;

};

#endif // __SCENE_H__