#ifndef __BATTLESCENE_H__
#define __BATTLESCENE_H__

#include "Module.h"
#include "Player.h"
#include "NPC.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;


enum CombatState
{
	SELECT_CHARACTER,
	SELECT_ACTION,
	SELECT_ENEMY,
	ENEMY_ATTACK,
	NONE
};

class BattleScene : public Module
{
public:

	BattleScene(bool startEnabled);

	// Destructor
	virtual ~BattleScene();

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

	void CheckState();

	void FindEnemyWithMostLife();

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;

	SDL_Rect playerIndicator;

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;

	// Test Dialogue
	Dialogue* testDialogue;
	Dialogue* testDialogue2;

public:
	CombatState combatState;

	bool playerWin = false;
	bool playerLose = false;

	bool hasStartedCombat;

	int currentPlayerInCombatIndex = 0;
	int currentEnemyInCombatIndex = 0;
	int currentEnemySelectedIndex = 0;
	int selectAttackIndex = 0;

	int timerEnemy = 0;

	
	
};

#endif // __BATTLESCENE_H__