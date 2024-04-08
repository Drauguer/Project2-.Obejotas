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

#include<iostream>
#include<cstdlib>
using namespace std;


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
		app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
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
				printf("that character is dead, please select another\n");
			}
		}

		app->render->DrawCircle(app->scene->allies[currentPlayerInCombatIndex]->position.x - 20, app->scene->allies[currentPlayerInCombatIndex]->position.y + 20, 5, 255, 0, 0, 255);

		break;
	case CombatState::SELECT_ACTION:

		// Codigo provisional para los "iconos" de los ataques

		for (int i = 0; i < 2; ++i)
		{
			app->render->DrawRectangle({ 400 + 100 * i, 600, 70, 70 }, 0, 255, 0, 255);
		}

		app->render->DrawCircle(430 + 100 * selectAttackIndex, 575, 15, 255, 0, 0, 255);

		//Navigate in the selection attack menu
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			if (selectAttackIndex < app->scene->allies[currentPlayerInCombatIndex]->numAttacks - 1)
			{
				selectAttackIndex += 1;
				
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			if (selectAttackIndex > 0)
			{
				selectAttackIndex -= 1;
			
			}
		}
		//Selected action
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->scene->allies[currentPlayerInCombatIndex]->CheckAttack(selectAttackIndex, currentPlayerInCombatIndex);
		}
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			printf("vuelves a la seleccion de personaje\n");
			combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case CombatState::SELECT_ENEMY:
		//Navigate in the selection enemy menu
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			if (currentEnemySelectedIndex + 1 < app->scene->enemies.Count())
			{
				app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = false;
				currentEnemySelectedIndex += 1;
				app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = true;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			if (currentEnemySelectedIndex - 1 >= 0)
			{
				app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = false;
				currentEnemySelectedIndex -= 1;
				app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = true;
			}
		}
		//Selected character, waiting for action
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			if (app->scene->enemies[currentEnemySelectedIndex]->life > 0)
			{
				app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = false;
				
				switch (selectAttackIndex)
				{
				case 0:
					app->scene->enemies[currentEnemySelectedIndex]->life -= app->scene->allies[currentPlayerInCombatIndex]->attack;
					combatState = CombatState::ENEMY_ATTACK;
					break;
				case 1:
					combatState = CombatState::ENEMY_ATTACK;
					break;
				}

			}
			else
			{
				printf("that enemy is dead, please select another\n");
			}
		}

		app->render->DrawCircle(app->scene->enemies[currentEnemySelectedIndex]->position.x + 75, app->scene->enemies[currentEnemySelectedIndex]->position.y + 40, 5, 255, 0, 0, 255);


		break;
	case CombatState::ENEMY_ATTACK:
		//Aqui va el ataque del enemigo y despues un check State
		timerEnemy++;
		srand((unsigned) time(NULL));
		int indexAttack = rand() % app->scene->allies.Count();

		if (app->scene->enemies[currentEnemyInCombatIndex]->life <= 0)
		{
			currentEnemyInCombatIndex++;
		}
		
		if (timerEnemy >= 120)
		{
			printf("ataque de enemigo\n");
			app->scene->allies[indexAttack]->life -= app->scene->enemies[currentEnemyInCombatIndex]->attack;
			timerEnemy = 0;
			currentEnemyInCombatIndex++;
		}

		if (currentEnemyInCombatIndex == app->scene->enemies.Count())
		{
			CheckState();
			currentEnemyInCombatIndex = 0;
			combatState = CombatState::SELECT_CHARACTER;
		}
		
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
		Disable();
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
		Disable();
		app->scene->isOnCombat = false;
		hasStartedCombat = false;
		return;
	}
}





