#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Item.h"
#include "NPC.h"
#include "ModuleFonts.h"
#include "BattleScene.h"
#include "BaseEnemy.h"
#include "BaseAlly.h"

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

	//L03: DONE 3b: Instantiate the player using the entity manager
	//L04 DONE 7: Get player paremeters
	player = (Player*) app->entityManager->CreateEntity(EntityType::PLAYER);
	//Assigns the XML node to a member in player
	player->parameters = config.child("player");
	player->config = config.child("player");

	//Get the map name from the config file and assigns the value in the module
	app->map->name = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();

	

	// iterate all items in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	// iterate NPCs in scene
	for (pugi::xml_node npcNode = config.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
	{
		NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
		npc->parameters = npcNode;
	}

	// iterate Enemies in scene
	for (pugi::xml_node enemyNode = config.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
	{
		BaseEnemy* enemy = (BaseEnemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemies.Add(enemy);
		enemy->parameters = enemyNode;
	}
	
	for (pugi::xml_node allyNode = config.child("ally"); allyNode; allyNode = allyNode.next_sibling("ally"))
	{
		BaseAlly* ally = (BaseAlly*)app->entityManager->CreateEntity(EntityType::ALLY);
		allies.Add(ally);
		ally->parameters = allyNode;
		
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/Music Loops/Wilderness-Daytime-1-_loop_.ogg", 2.0f);

	//SFX loading
	/*encounterFx = app->audio->LoadFx(scene_parameter.child("encounterFx").attribute("audiopath").as_string());*/
	encounterFx = app->audio->LoadFx("Assets/Audio/Fx/10_Battle_SFX/55_Encounter_02.ogg");
	/*walkingRockFx = app->audio->LoadFx("Assets/Audio/Fx/12_Player_Movement_SFX/08Steprock02.wav");*/

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene

	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20};
	gcButtom = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);

	SDL_Rect ExitButton = { windowW / 2 - 60,windowH / 2 + 120, 240, 80 };
	exitScene = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Pause", ExitButton, this);
	exitScene->state = GuiControlState::DISABLED;
	/*testDialogue = (Dialogue*)app->dialogueManager->CreateDialogue("hello world!", DialogueType::PLAYER);
	testDialogue2 = (Dialogue*)app->dialogueManager->CreateDialogue("diabloooo que pasaa ", DialogueType::PLAYER);*/

	
	


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
			if (player->position.y > mapLimitY) {
				app->render->camera.y = (-mapLimitY * scale) + app->win->screenSurface->h / 2;
			}
			else
			{
				app->render->camera.y = (-player->position.y * scale) + app->win->screenSurface->h / 2;

			}
		}
	}

	
	
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
		app->scene->player->isOnPause = true;
		
		app->scene->exitScene->state = GuiControlState::NORMAL;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		
		isOnCombat = !isOnCombat;
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		this->Disable();
		app->battleScene->Enable();
		app->audio->PlayFx(encounterFx);
	}




	// Get the mouse position and obtain the map coordinate
	iPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	iPoint mouseTile = app->map->WorldToMap(mousePos.x - app->render->camera.x,
										    mousePos.y - app->render->camera.y);
	
	// Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	iPoint origin = iPoint(2, 2);

	//If mouse button is pressed modify player position
	/*if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		player->position = iPoint(highlightedTileWorld.x, highlightedTileWorld.y);
		app->map->pathfinding->CreatePath(origin, mouseTile);
	}*/

	// L13: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}
	
	// L14: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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

	}
	if (control->id == 2) {
		app->scene->player->isOnPause = false;
		exitScene->state = GuiControlState::DISABLED;
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



