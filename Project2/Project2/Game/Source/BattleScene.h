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

	int FindFirstEnemyIndex();

	bool CheckAllPlayersAttacked();

	void SetAllPlayersAliveToAttack();

	int FindFirstPlayerToAttackIndex();

	int FindFirstPlayerIndex();

	void TextAttack(int attackIndex);

	void CheckEnemyAbility(int abilityId, int playerToAttackIndex = -1);

	void GiveReward(int npcID);

	void PlayerLose();
	void PlayerWin();


private:
	int uiGamePadCounter = 0;

	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;

	SDL_Rect playerIndicator;

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;

	SDL_Rect dialogueBoxPos;
	SDL_Rect dialogueBoxPos2;

	// Test Dialogue
	Dialogue* testDialogue;
	Dialogue* testDialogue2;

	// Attack Icons textures
	SDL_Texture* sableLaser;
	SDL_Texture* gritoGuerra;
	SDL_Texture* laserGun;
	SDL_Texture* fireBall;
	SDL_Texture* healingMagic;
	SDL_Texture* laserCannon;
	SDL_Texture* martillazo;
	SDL_Texture* elMuro;
	SDL_Texture* lluviaAsesina;
	SDL_Texture* inspiracionBardica;
	SDL_Texture* vivaRock;
	SDL_Texture* musicaMaldita;

	// Vidas Personajes
	SDL_Texture* lifeMago;
	SDL_Texture* lifeEnano;
	SDL_Texture* lifeEldrin;
	SDL_Texture* lifeBardo;
	SDL_Texture* lifeEnemy;

	// Fondo Mapa
	SDL_Texture* background;


	GuiControlButton* exitBattle;
	GuiControlButton* continueBattle;

	bool isExiting = false;

	bool hasEndedCombat = false;


public:
	SDL_Texture* arrowTexture = NULL;
	SDL_Texture* winTexture = NULL;
	SDL_Texture* loseTexture = NULL;
	const char* arrowTexturePath;
	const char* winTexturePath;
	const char* loseTexturePath;
	Animation* currentArrowAnim;
	Animation* currentFinalAnimation;

	Animation idleArrowAnim;
	Animation winAnim;
	Animation loseAnim;

	CombatState combatState;

	bool playerWin = false;
	bool playerLose = false;

	bool hasStartedCombat;

	int currentPlayerInCombatIndex = 0;
	int currentEnemyInCombatIndex = 0;
	int currentEnemySelectedIndex = 0;
	int selectAttackIndex = 0;
	int idAttack = 0;

	int timerEnemy = 0;

	float damage;

	//Cooldown for selecting abilities
	//float selectionCooldown = 1;

	uint hoverFx;
	uint clickFx;
	uint declineFx;
	uint deniedFx;

	// NPC ID para detectar que NPC ha empezado el combate
	int npcIDbattle = 0;

	// Timer para textbox
	bool isText = false;
	int timerText = 0;
	
};

#endif // __BATTLESCENE_H__