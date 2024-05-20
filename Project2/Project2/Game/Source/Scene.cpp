#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "MainMenu.h"
#include "Map.h"
#include "Physics.h"
#include "Item.h"
#include "NPC.h"
#include "ModuleFonts.h"
#include "BattleScene.h"
#include "BaseEnemy.h"
#include "BaseAlly.h"
#include "PuzlePilar.h"
#include "PuzleArrows.h"
#include "PuzlePassword.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "DialogueManager.h"
#include <string>
#include "SDL_mixer/include/SDL_mixer.h"

#include <iostream>

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node config)
{
	LOG("Loading Scene");
	bool ret = true;

	scene_parameter = config;

	//L03: DONE 3b: Instantiate the player using the entity manager
	//L04 DONE 7: Get player paremeters
	player = (Player*) app->entityManager->CreateEntity(EntityType::PLAYER);
	//Assigns the XML node to a member in player
	player->parameters = config.child("player");
	player->config = config.child("player");

	//Get the map name from the config file and assigns the value in the module

	switch (mapID)
	{
	case 0:
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
		break;
	case 1:
		app->map->name = config.child("map2").attribute("name").as_string();
		app->map->path = config.child("map2").attribute("path").as_string();
		break;
	case 2:
		app->map->name = config.child("mapInterior").attribute("name").as_string();
		app->map->path = config.child("mapInterior").attribute("path").as_string();
		break;
	case 3:
		app->map->name = config.child("mapInterior2").attribute("name").as_string();
		app->map->path = config.child("mapInterior2").attribute("path").as_string();
		break;
	case 4:
		app->map->name = config.child("mapInterior3").attribute("name").as_string();
		app->map->path = config.child("mapInterior3").attribute("path").as_string();
		break;
	case 5:
		app->map->name = config.child("mapZona2").attribute("name").as_string();
		app->map->path = config.child("mapZona2").attribute("path").as_string();
		break;
	case 6:
		app->map->name = config.child("mapZona3").attribute("name").as_string();
		app->map->path = config.child("mapZona3").attribute("path").as_string();
		break;
	case 7:
		app->map->name = config.child("mapPuzzle1").attribute("name").as_string();
		app->map->path = config.child("mapPuzzle1").attribute("path").as_string();
		break;
	case 8:
		app->map->name = config.child("mapPuzzle2").attribute("name").as_string();
		app->map->path = config.child("mapPuzzle2").attribute("path").as_string();
		break;

	}
	

	

	// iterate all items in the scene
	// Check https://pugixml.org/docs/quickstart.html#access


	/*for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/

	


	// iterate NPCs in scene
	for (pugi::xml_node npcNode = config.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
	{
		if (npcNode.attribute("mapID").as_int() == mapID)
		{
			NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
			npcs.Add(npc);
			npc->parameters = npcNode;

			
		}
		
	}
	for (pugi::xml_node pilarNode = config.child("puzlePilar"); pilarNode; pilarNode = pilarNode.next_sibling("puzlePilar"))
	{
		if (pilarNode.attribute("mapID").as_int() == mapID)
		{
			PuzlePilar* pilar = (PuzlePilar*)app->entityManager->CreateEntity(EntityType::PUZLE_PILAR);
			puzlePilars.Add(pilar);
			pilar->parameters = pilarNode;


		}

	}
	for (pugi::xml_node arrowsNode = config.child("puzleArrows"); arrowsNode; arrowsNode = arrowsNode.next_sibling("puzleArrows"))
	{
		if (arrowsNode.attribute("mapID").as_int() == mapID)
		{
			PuzleArrows* passwordPuzle = (PuzleArrows*)app->entityManager->CreateEntity(EntityType::PUZLE_ARROWS);
			passwordPuzle->parameters = arrowsNode;


		}

	}
	for (pugi::xml_node passwordNode = config.child("puzlePassword"); passwordNode; passwordNode = passwordNode.next_sibling("puzlePassword"))
	{
		if (passwordNode.attribute("mapID").as_int() == mapID)
		{
			PuzlePassword* passwordPuzle = (PuzlePassword*)app->entityManager->CreateEntity(EntityType::PUZLE_PASSWORD);
			passwordPuzle->parameters = passwordNode;


		}

	}

	if (loadAllies)
	{
		
		LoadItems();
		
	}
	
	

	return ret;
}

void Scene::LoadEnemies()
{
	//Esto es provisional, en futuras entregas se guardara las estadisticas de los players/enemies
	if (enemies.start != NULL)
	{
		/*for (int i = 0; i < enemies.Count(); i++)
		{
			app->physics->DestroyObject(enemies[i]->pbody);

		}*/
		enemies.Clear();
	}

	// iterate Enemies in combat
	for (pugi::xml_node enemyNode = scene_parameter.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
	{
		if (enemyNode.attribute("combatID").as_int() == combatID)
		{
			BaseEnemy* enemy = (BaseEnemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
			enemies.Add(enemy);
			enemy->parameters = enemyNode;
			enemy->Start();
		}
		
	}
}

void Scene::LoadAllies()
{
	//Esto es provisional, en futuras entregas se guardara las estadisticas de los players/enemies
	if (allies.start != NULL)
	{
		for (int i = 0; i < allies.Count(); i++)
		{
			app->physics->DestroyObject(allies[i]->pbody);

		}
		allies.Clear();
	}
	

	// iterate Allies in combat
	for (pugi::xml_node allyNode = scene_parameter.child("eldrin"); allyNode; allyNode = allyNode.next_sibling("eldrin"))
	{
		BaseAlly* ally = (BaseAlly*)app->entityManager->CreateEntity(EntityType::ALLY);
		allies.Add(ally);
		ally->parameters = allyNode;
		ally->Start();

	}

	/*for (pugi::xml_node allyNode = scene_parameter.child("ally2"); allyNode; allyNode = allyNode.next_sibling("ally2"))
	{
		BaseAlly* ally = (BaseAlly*)app->entityManager->CreateEntity(EntityType::ALLY);
		allies.Add(ally);
		ally->parameters = allyNode;
		ally->Start();

	}*/

	// Testing inventory, will be deleted
	/*allies[0]->inventoryChar.Add(item1);
	allies[0]->inventoryChar.Add(item2);
	allies[1]->inventoryChar.Add(item3);
	allies[1]->inventoryChar.Add(item4);
	allies[0]->inventoryChar.Add(item5);*/
	
}

void Scene::LoadItems()
{

	for (pugi::xml_node itemNode = scene_parameter.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		listItems.Add(item);
		item->parameters = itemNode;
		item->Start();

	}

	pugi::xml_node itemMageNode = scene_parameter.child("itemMage");
	itemMage = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	itemMage->parameters = itemMageNode;
	listItems.Add(itemMage);

	pugi::xml_node itemQuest3Node = scene_parameter.child("itemQuest3");
	itemQuest3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	itemQuest3->parameters = itemQuest3Node;
	listItems.Add(itemQuest3);

	pugi::xml_node itemNum1Node = scene_parameter.child("itemNum1");
	itemNum1 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	itemNum1->parameters = itemNum1Node;
	listItems.Add(itemNum1);

	pugi::xml_node itemNum2Node = scene_parameter.child("itemNum2");
	itemNum2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	itemNum2->parameters = itemNum2Node;
	listItems.Add(itemNum2);

	pugi::xml_node itemNum3Node = scene_parameter.child("itemNum3");
	itemNum3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	itemNum3->parameters = itemNum3Node;
	listItems.Add(itemNum3);
	
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/Music Loops/Scene1Music.ogg", 2.0f);

	//SFX loading
	/*encounterFx = app->audio->LoadFx(scene_parameter.child("encounterFx").attribute("audiopath").as_string());*/
	encounterFx = app->audio->LoadFx("Assets/Audio/Fx/10_Battle_SFX/55Encounter02.wav");
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/013Confirm03.wav");
	

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene

	
	SDL_Rect ResumeButton = { windowW / 2 - 60,windowH / 2 - 120, 240, 80 };

	ResumeScene = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Resume", ResumeButton, this);
	ResumeScene->state = GuiControlState::DISABLED;
	/*testDialogue = (Dialogue*)app->dialogueManager->CreateDialogue("hello world!", DialogueType::PLAYER);
	testDialogue2 = (Dialogue*)app->dialogueManager->CreateDialogue("diabloooo que pasaa ", DialogueType::PLAYER);*/
	
	SDL_Rect ExitButton_ = { windowW / 2 - 60,windowH / 2 +120, 240, 80 };
	exitScene = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton_, this);
	exitScene->state = GuiControlState::DISABLED;

	SDL_Rect SettingsSceneButton = { windowW / 2 - 60,windowH / 2, 240, 80 };
	settingsScene = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", SettingsSceneButton, this);
	settingsScene->state = GuiControlState::DISABLED;

	SDL_Rect Return_Initial = { windowW / 2 - 60,windowH / 2 + 120, 340, 80 };
	Initial_Screen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Return to inicial screen", Return_Initial, this);
	Initial_Screen->state = GuiControlState::DISABLED;

	SDL_Rect FullScreenCheck = { windowW / 2 + 180,windowH / 2 - 240, 240, 80 };
	FullScreen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Full Screen", FullScreenCheck, this);
	FullScreen->state = GuiControlState::DISABLED;

	SDL_Rect FullScreenCheckOff = { windowW / 2 - 60,windowH / 2-240, 240, 80 };
	FullScreenOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Full Screen Off", FullScreenCheckOff, this);
	FullScreenOff->state = GuiControlState::DISABLED;

	SDL_Rect VsincCheck = { windowW / 2 - 400,windowH / 2 - 240 , 240, 80 };
	Vsinc = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Vsync", VsincCheck, this);
	Vsinc->state = GuiControlState::DISABLED;

	SDL_Rect VsincCheckOff = { windowW / 2 - 400,windowH / 2 - 320 , 240, 80 };
	VsincOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Vsync Off", VsincCheckOff, this);
	VsincOff->state = GuiControlState::DISABLED;

	SDL_Rect Go_Back = { windowW / 2 -400,windowH / 2 + 140, 240, 80 };
	returned = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "Go Back", Go_Back, this);
	returned->state = GuiControlState::DISABLED;

	SDL_Rect SoundOff = { windowW / 2 - 400,windowH / 2 - 60,240,80 };
	AudioSceneOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "Audio Off", SoundOff, this);
	AudioSceneOff->state = GuiControlState::DISABLED;

	SDL_Rect SoundOn = { windowW / 2 - 400,windowH / 2 - 140,240,80 };
	AudioSceneOn = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Audio On", SoundOn, this);
	AudioSceneOn->state = GuiControlState::DISABLED;

	SDL_Rect EffectOff = { windowW / 2 + 180,windowH / 2 - 60,240,80 };
	FxSceneOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Fx Off", EffectOff, this);
	FxSceneOff->state = GuiControlState::DISABLED;

	SDL_Rect EffectOn = { windowW / 2 + 180,windowH / 2 - 140,240,80 };
	FxSceneOn = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "Fx On", EffectOn, this);
	FxSceneOn->state = GuiControlState::DISABLED;

	if (loadAllies)
	{
		// Instantiate Allies
		LoadAllies();
		loadAllies = false;
	}

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (SDL_Init(SDL_INIT_TIMER) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Prueba para añadir allies
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		for (pugi::xml_node allyNode = scene_parameter.child("bardo"); allyNode; allyNode = allyNode.next_sibling("bardo"))
		{
			BaseAlly* ally = (BaseAlly*)app->entityManager->CreateEntity(EntityType::ALLY);
			allies.Add(ally);
			ally->parameters = allyNode;
			ally->Start();

		}

		for (pugi::xml_node allyNode = scene_parameter.child("mage"); allyNode; allyNode = allyNode.next_sibling("mage"))
		{
			BaseAlly* ally = (BaseAlly*)app->entityManager->CreateEntity(EntityType::ALLY);
			allies.Add(ally);
			ally->parameters = allyNode;
			ally->Start();

		}

		for (pugi::xml_node allyNode = scene_parameter.child("enano"); allyNode; allyNode = allyNode.next_sibling("enano"))
		{
			BaseAlly* ally = (BaseAlly*)app->entityManager->CreateEntity(EntityType::ALLY);
			allies.Add(ally);
			ally->parameters = allyNode;
			ally->Start();

		}
	}


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		if (!isFullScreen) {
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
			isFullScreen = true;
		}
		else
		{
			uint height = 0;
			uint width = 0;
			app->win->GetWindowSize(width, height);
			SDL_SetWindowSize(app->win->window, width, height);
			SDL_SetWindowFullscreen(app->win->window, 0);
			isFullScreen = false;
		}
	}
	int scale = app->win->GetScale();

	//L02 DONE 3: Make the camera movement independent of framerate
	float camSpeed = 1; 

	if (isOnDebugMode)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(camSpeed * dt);
	}
	else
	{
		mapLimitX = app->map->mapData.width * app->map->mapData.tilewidth - app->win->screenSurface->w / (2 * scale);
		mapLimitY = app->map->mapData.height * app->map->mapData.tileheight - app->win->screenSurface->h / (2 * scale);

		//Moving camera function
		if (player->position.x > app->win->screenSurface->w / (2 * scale)) {
			if (player->position.x > mapLimitX) {
				app->render->camera.x = (-mapLimitX * scale) + app->win->screenSurface->w / 2;
			}
			else
			{
				app->render->camera.x = (-player->position.x * scale) + app->win->screenSurface->w / 2;

			}
		}
		if (player->position.y > app->win->screenSurface->h / (2 * scale)) {
			
			if (player->position.y > mapLimitY) 
			{
				app->render->camera.y = (-mapLimitY * scale) + app->win->screenSurface->h / 2;
			}
			else
			{
				app->render->camera.y = (-player->position.y * scale) + app->win->screenSurface->h / 2;

			}
		}
	}

	
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->scene->player->isOnPause == false) {
		app->scene->player->isOnPause = true;
		app->scene->ResumeScene->state = GuiControlState::NORMAL;
		app->scene->exitScene->state = GuiControlState::NORMAL;
		app->scene->settingsScene->state = GuiControlState::NORMAL;
		
		Mix_PauseMusic();
	}
	
	if (app->scene->player->isOnPause == false)
	{
		Mix_ResumeMusic();
	}
	
	
	/*if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		
		isOnCombat = !isOnCombat;
		LoadEnemies();
		LoadAllies();
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		this->Disable();
		app->battleScene->Enable();
		app->audio->PlayFx(encounterFx);
	}*/




	
	
	// L14: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(IsExiting == true)
		ret = false;

	return ret;
}


// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

iPoint Scene::GetPLayerPosition() {
	return player->position;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);
	bool ret = true;

	if (control->id == 1) {

		app->scene->player->isOnPause = false;
		exitScene->state = GuiControlState::DISABLED;
		settingsScene->state = GuiControlState::DISABLED;
		ResumeScene->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 2) {
		app->audio->PlayFx(clickFx);
		IsExiting = true;
	}
	if (control->id == 3) {
		ResumeScene->state = GuiControlState::DISABLED;
		settingsScene->state = GuiControlState::DISABLED;
		Initial_Screen->state = GuiControlState::DISABLED;
		FullScreen->state = GuiControlState::NORMAL;
		Vsinc->state = GuiControlState::NORMAL;
		returned->state = GuiControlState::NORMAL;
		AudioSceneOff->state = GuiControlState::NORMAL;
		FxSceneOff->state = GuiControlState::NORMAL;
		exitScene->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 4) {
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
		FullScreenOff->state = GuiControlState::NORMAL;
		FullScreen->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 5) {
		app->win->GetWindowSize(windowW, windowH);
		SDL_SetWindowSize(app->win->window, windowW, windowH);
		SDL_SetWindowFullscreen(app->win->window, 0);

		FullScreen->state = GuiControlState::NORMAL;
		FullScreenOff->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 6) {
		app->IsVsincActive = false;
		Vsinc->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::NORMAL;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 7) {
		app->IsVsincActive = true;
		Vsinc->state = GuiControlState::NORMAL;
		VsincOff->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 8) {
		FullScreen->state = GuiControlState::DISABLED;
		FullScreenOff->state = GuiControlState::DISABLED;
		Vsinc->state = GuiControlState::DISABLED;
		Initial_Screen->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::DISABLED;
		returned->state = GuiControlState::DISABLED;
		AudioSceneOff->state = GuiControlState::DISABLED;
		AudioSceneOn->state = GuiControlState::DISABLED;
		FxSceneOff->state = GuiControlState::DISABLED;
		FxSceneOn->state = GuiControlState::DISABLED;
		ResumeScene->state = GuiControlState::NORMAL;
		exitScene->state = GuiControlState::NORMAL;
		settingsScene->state = GuiControlState::NORMAL;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 9) {
		AudioSceneOff->state = GuiControlState::DISABLED;
		AudioSceneOn->state = GuiControlState::NORMAL;
		app->audio->SetMusicVolume(0.0f);

		app->audio->PlayFx(clickFx);

		
	}
	if (control->id == 10) {
		AudioSceneOff->state = GuiControlState::NORMAL;
		AudioSceneOn->state = GuiControlState::DISABLED;
		app->audio->SetMusicVolume(100.0f);

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 11) {
		app->scene->player->isOnPause = false;
		app->entityManager->Disable();
		
		app->scene->exitScene->state = GuiControlState::DISABLED;
		app->scene->ResumeScene->state = GuiControlState::DISABLED;
		app->scene->Initial_Screen->state = GuiControlState::DISABLED;
		app->scene->settingsScene->state = GuiControlState::DISABLED;
		app->scene->Disable();
		app->physics->Disable();
		app->map->Disable();
		app->audio->CleanUp();
		app->AwakeAudio();
		app->mainMenu->Enable();
		app->mainMenu->Start();
		
	}
	if (control->id == 12) {
		FxSceneOff->state = GuiControlState::DISABLED;
		FxSceneOn->state = GuiControlState::NORMAL;
		app->audio->SetFxVolume(0.0f);

		app->audio->PlayFx(clickFx);
	}

	if (control->id == 13) {
		FxSceneOff->state = GuiControlState::NORMAL;
		FxSceneOn->state = GuiControlState::DISABLED;
		app->audio->SetFxVolume(100.0f);

		app->audio->PlayFx(clickFx);
	}

	return true;
}

void Scene::Enable()
{
	if (!active)
	{
		active = true;

		Start();
	}
}

void Scene::Disable()
{
	if (active)
	{
		active = false;
		CleanUp();
	}
}


void Scene::CheckPilarPuzleResult()
{
	for (int i = 0; i < puzlePilars.Count(); i++)
	{
		if (puzlePilars[i]->id != combination[i])
		{
			combination.Clear();
			for (int j = 0; j < puzlePilars.Count(); j++)
			{
				puzlePilars[j]->isTriggered = false;
			}
			return;
		}
	}
	PilarPuzleSuccess();
}

void Scene::PilarPuzleSuccess()
{
	app->hasSolvedPilarPuzzle = true;
}

bool Scene::CheckAllPilars()
{
	for (int j = 0; j < puzlePilars.Count(); j++)
	{
		if (!puzlePilars[j]->isTriggered) {
			return false;
		}
	}
	return true;
}



