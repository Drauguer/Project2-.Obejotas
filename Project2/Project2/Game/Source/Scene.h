#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Mago.h"
#include "NPC.h"
#include "BaseAlly.h"
#include "BaseEnemy.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "PuzlePilar.h"
#include <string>


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

	// Called before quittingr
	bool CleanUp();

	// Return the player position
	iPoint GetPLayerPosition();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Enable Module
	void Enable();
	
	// Disable Module
	void Disable();

	// Load Enemies
	void LoadEnemies();

	// Load Allies
	void LoadAllies();

	// Load Items
	void LoadItems();

	void CheckPilarPuzleResult();

	void PilarPuzleSuccess();

	bool CheckAllPilars();

	void GamePadUi(List<GuiControlButton*> members, bool isOnSettings);

	//el resultado se hace al dar enter, se hace un switch del id del boton actual 
	//Para navegar es mas complicado deberia haber una lista de todos los botones actuales 

	pugi::xml_node scene_parameter;
	bool IsExiting = false;
	bool isOnSettings = false;

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;

	int uiGamePadCounter = 0;
	
	bool IsVsyncActive = false;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;
	GuiControlButton* exitScene;
	GuiControlButton* ResumeScene;
	GuiControlButton* settingsScene;
	GuiControlButton* FullScreen;
	GuiControlButton* FullScreenOff;
	GuiControlButton* Vsinc;
	GuiControlButton* VsincOff;
	GuiControlButton* AudioSceneOff;
	GuiControlButton* AudioSceneOn;
	GuiControlButton* FxSceneOff;
	GuiControlButton* FxSceneOn;
	GuiControlButton* returned;
	GuiControlButton* Initial_Screen;

	

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;
	

	// Test Dialogue
	Dialogue* testDialogue;
	Dialogue* testDialogue2;

public:



	//L03: DONE 3b: Declare a Player attribute
	Player* player;
	Mago* mago;

	List<PuzlePilar*> puzlePilars;
	List<int> combination;

	BaseAlly* playerInCombat;
	BaseAlly* allyInCombat1;
	List<BaseAlly*> allies;

	// List of all NPC
	List<NPC*> npcs;
	List<int> npcIDcombatFinished;

	//Audio
	uint encounterFx;
	uint clickFx;
	unsigned int hoverFx;
	unsigned int declineFx;
	unsigned int deniedFx;
	unsigned int attackSFX;
	unsigned int fireMagicAttackSFX;
	unsigned int hpRecoverSFX;
	unsigned int attackUpSFX;
	unsigned int firePuzzleSFX;
	unsigned int arrowPuzzleSFX;
	unsigned int pauseSFX;
	unsigned int unpauseSFX;

	//List of enemies (reemplazar porque no son players xd) 
	List<BaseEnemy*> enemies;


	bool isOnCombat = false;

	bool hasPressedE;

	//Fullscreen
	bool isFullScreen = false;

	//DebugMode
	bool isOnDebugMode = false;

	//limit of the map 
	int mapLimitX = 1600;
	int mapLimitY = 780;

	// ID for Load map
	int mapID = 0;
	int playerMapID = 0;

	bool isPlayerinPuzzle1 = false;
	bool isPlayerinPuzzle2 = false;
	/*bool hasPlayerEnteredLevel2 = false;*/
	bool isPlayerinLevel2Room1 = false;
	bool hasEnteredBoss = false;

	// ID for combats
	int combatID = 0;

	bool changedMap;

	// Testing items
	
	Item* itemMage;
	Item* itemQuest3;
	Item* itemNum1;
	Item* itemNum2;
	Item* itemNum3;

	List<Item*> listItems;

	
	bool loadAllies = true;

	//music switching
	bool isZone3MusicPlayed = false;
	bool isZone2MusicPlayed = false;
	bool isZone1MusicPlayed = false;
	
	
	SDL_Texture* IdleStartButton;
	SDL_Texture* IdleContinueButton;
	SDL_Texture* IdleOptionsButton;
	SDL_Texture* IdleExitButton;
	SDL_Texture* IdleVsyncButton;
	SDL_Texture* IdleFullScreenButton;
	SDL_Texture* IdleNormalScreenButton;
	SDL_Texture* IdleBackButton;

	// Inventory icon
	SDL_Texture* inventoryIdle;
	SDL_Texture* inventoryNewItem;
	bool isNewItem = false;

	// Endscreen
	bool isEnd = false;
};

#endif // __SCENE_H__