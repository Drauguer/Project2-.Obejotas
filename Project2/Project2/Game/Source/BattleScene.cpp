#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "BattleScene.h"
#include "Map.h"
#include "Item.h"
#include "NPC.h"
#include "ModuleFonts.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "DialogueManager.h"

BattleScene::BattleScene(bool startEnabled) : Module(startEnabled)
{
	name.Create("battleScene");
}

// Destructor
BattleScene::~BattleScene()
{}

// Called before render is available
bool BattleScene::Awake(pugi::xml_node config)
{
	LOG("Loading Scene");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool BattleScene::Start()
{
	combatState = CombatState::NONE;
	return true;
}

// Called each loop iteration
bool BattleScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool BattleScene::Update(float dt)
{
	if (app->scene->isOnCombat && !hasStartedCombat) 
	{
		combatState = CombatState::SELECT_CHARACTER;
		app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = true;
		hasStartedCombat = true;
	}

	switch (combatState)
	{
	case CombatState::SELECT_CHARACTER:
		//Navigate in the selection character menu
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) 
		{
			if (currentPlayerInCombatIndex + 1 < app->scene->allies.Count())
			{
				app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
				currentPlayerInCombatIndex +=1;
				app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = true;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) 
		{
			if (currentPlayerInCombatIndex - 1 >= 0)
			{
				app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
				currentPlayerInCombatIndex -=1;
				app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = true;
			}
		}
		//Selected character, waiting for action
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			if (app->scene->allies[currentPlayerInCombatIndex]->life > 0)
			{
				app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
				combatState = CombatState::SELECT_ACTION;
			}
			else
			{
				printf("that character is dead, please select another");
			}
		}
		break;
	case CombatState::SELECT_ACTION:
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{

			printf("Bola de fuego, hace su daño al primer enemigo");
			app->scene->enemies[0]->life -= app->scene->allies[currentPlayerInCombatIndex]->attack;
			CheckState();
			combatState = CombatState::ENEMY_ATTACK;

		}
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			printf("curacion, obtiene 10 de vida");
			app->scene->allies[currentPlayerInCombatIndex]->life += 10;
			combatState = CombatState::ENEMY_ATTACK;

		}
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			printf("vuelves a la seleccion de personaje");
			combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case CombatState::ENEMY_ATTACK:
		//Aqui va el ataque del enemigo y despues un check State
		CheckState();
		combatState = CombatState::SELECT_CHARACTER;
		break;
	default:
		break;
	}

	return true;
}

// Called each loop iteration
bool BattleScene::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool BattleScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool BattleScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

void BattleScene::Enable()
{
	if (!active)
	{
		active = true;

		Start();
	}
}

void BattleScene::Disable()
{
	if (active)
	{
		active = false;
		CleanUp();
	}
}

void BattleScene::CheckState()
{
	playerLose = true;
	playerWin = true;
	for (int i = 0; i < app->scene->allies.Count(); i++)
	{
		if (app->scene->allies[i]->life > 0)
		{
			playerLose = false;
			break;
		}
	}
	if (playerLose) {
		printf("Has perdido el combate");
		app->scene->Enable();
		app->scene->isOnCombat = false;
		hasStartedCombat = false;
		return;
	}
	for (int i = 0; i < app->scene->enemies.Count(); i++)
	{
		if (app->scene->enemies[i]->life > 0)
		{
			playerWin = false;
			break;
		}
	}
	if (playerWin) {
		printf("Has ganado el combate");
		app->scene->Enable();
		app->scene->isOnCombat = false;
		hasStartedCombat = false;
		return;
	}
}



