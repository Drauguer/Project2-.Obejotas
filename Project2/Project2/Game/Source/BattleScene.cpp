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
#include "BaseAlly.h"
#include "ModuleFonts.h"
#include "Physics.h"

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


	arrowTexturePath = config.attribute("texturePath").as_string();




	for (pugi::xml_node node = config.child("arrow").child("pushback"); node; node = node.next_sibling("pushback")) {
		idleArrowAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	idleArrowAnim.speed = config.child("arrow").attribute("animspeed").as_float();
	idleArrowAnim.loop = config.child("arrow").attribute("loop").as_bool();

	bool ret = true;


	return ret;
}

// Called before the first frame
bool BattleScene::Start()
{
	hoverFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/001Hover01.wav");
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/013Confirm03.wav");
	declineFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/029Decline09.wav");
	deniedFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/033Denied03.wav");

	sableLaser = app->tex->Load("Assets/Textures/LaserSwords.png");
	gritoGuerra = app->tex->Load("Assets/Textures/Scream_Icon.png");
	laserGun = app->tex->Load("Assets/Textures/lasergun_Icon.png");
	fireBall = app->tex->Load("Assets/Textures/FireBall_Icon.png");
	healingMagic = app->tex->Load("Assets/Textures/MagicHealth_Icon.png");
	laserCannon = app->tex->Load("Assets/Textures/laserbeam_Icon.png");
	martillazo = app->tex->Load("Assets/Textures/RedMace.png");
	lifeMago = app->tex->Load("Assets/Textures/LifeMago.png");
	lifeEnano = app->tex->Load("Assets/Textures/LifeEnano.png");
	lifeEnemy = app->tex->Load("Assets/Textures/LifeEnemy.png");
	background = app->tex->Load("Assets/Maps/FondoCombate.png");

	arrowTexture = app->tex->Load(arrowTexturePath);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

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
	GamePad& pad = app->input->pads[0];

	int scale = app->win->GetScale();
	if (app->scene->isOnCombat) 
	{

		app->render->camera.x = 0;
		app->render->camera.y = 0;

		int winW = windowW;
		int winH = windowH;
		dialogueBoxPos = { (winW / 2 - 600 - app->render->camera.x) / scale, (winH / 2 + 120 - app->render->camera.y) / scale, 1200 / scale, 250 / scale };
		dialogueBoxPos2 = { (winW / 2 - 600) / scale, (winH / 2 + 120) / scale, 1200 / scale, 250 / scale };

		app->render->DrawTexture(background, 0, 0);

		for (int i = 0; i < app->scene->allies.Count(); ++i)
		{

			if (strcmp(app->scene->allies[i]->charName.GetString(), "Gale el Mago") == 0)
			{
				app->render->DrawRectangle(app->scene->allies[i]->healthBar, 0, 255, 0, 255);
				app->render->DrawTexture(lifeMago, (app->scene->allies[i]->position.x - 150) / scale, app->scene->allies[i]->position.y / scale);
				
			}
			else if (strcmp(app->scene->allies[i]->charName.GetString(), "David el Enano") == 0)
			{
				app->render->DrawRectangle(app->scene->allies[i]->healthBar, 0, 255, 0, 255);
				app->render->DrawTexture(lifeEnano, (app->scene->allies[i]->position.x - 150) / scale, app->scene->allies[i]->position.y / scale);
				
			}
		}

		for (int i = 0; i < app->scene->enemies.Count(); ++i)
		{
			app->render->DrawRectangle(app->scene->enemies[i]->healthBar, 0, 255, 0, 255);
			app->render->DrawTexture(lifeEnemy, (app->scene->enemies[i]->position.x + 100) / scale, app->scene->enemies[i]->position.y / scale);
		}

		currentArrowAnim = &idleArrowAnim;
		//Timer for waiting to select action
		if (selectActionCooldown > 0) {
			selectActionCooldown--;
		}
		//Initial values
		if (!hasStartedCombat)
		{
			CheckState();
			combatState = CombatState::SELECT_CHARACTER;
			app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = true;
			app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
			hasStartedCombat = true;
		}
		//Combat state
		switch (combatState)
		{
		case CombatState::SELECT_CHARACTER:
			//Navigate in the selection character menu
			if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.l_y > 0 && selectActionCooldown == 0)
			{
				if (currentPlayerInCombatIndex + 1 < app->scene->allies.Count() &&
					app->scene->allies[currentPlayerInCombatIndex + 1]->life > 0 &&
					!app->scene->allies[currentPlayerInCombatIndex + 1]->hasAttacked)
				{
					app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
					currentPlayerInCombatIndex += 1;
					app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = true;

					app->audio->PlayFx(hoverFx);
				}
				selectActionCooldown = 10;
				
			}
			if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.l_y < 0 && selectActionCooldown == 0)
			{
				if (currentPlayerInCombatIndex - 1 >= 0 &&
					app->scene->allies[currentPlayerInCombatIndex - 1]->life > 0 &&
					!app->scene->allies[currentPlayerInCombatIndex - 1]->hasAttacked)
				{
					app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
					currentPlayerInCombatIndex -= 1;
					app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = true;

					app->audio->PlayFx(hoverFx);
				}
				selectActionCooldown = 10;
				
			}
			//Selected character, waiting for action
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || pad.a && selectActionCooldown == 0)
			{
				selectActionCooldown = 10;
				if (app->scene->allies[currentPlayerInCombatIndex]->life > 0)
				{
					app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
					combatState = CombatState::SELECT_ACTION;

					app->audio->PlayFx(clickFx);
				}
				else
				{
					printf("that character is dead, please select another\n");

					app->audio->PlayFx(deniedFx);
				}
				
			}

			//app->render->DrawCircle((app->scene->allies[currentPlayerInCombatIndex]->position.x - 20) / scale, (app->scene->allies[currentPlayerInCombatIndex]->position.y + 20) / scale, 5, 255, 0, 0, 255);
			currentArrowAnim->Update();
			app->render->DrawTexture(arrowTexture, (app->scene->allies[currentPlayerInCombatIndex]->position.x + 20) / scale, (app->scene->allies[currentPlayerInCombatIndex]->position.y ) / scale, &currentArrowAnim->GetCurrentFrame());


			break;
		case CombatState::SELECT_ACTION:

			// Codigo provisional para los "iconos" de los ataques

			for (int i = 0; i < app->scene->allies[currentPlayerInCombatIndex]->numAttacks; ++i)
			{

				switch (app->scene->allies[currentPlayerInCombatIndex]->abilities[i].id)
				{
				case 0:
					app->render->DrawTexture(sableLaser, (400 + 200 * i) / scale, 525 / scale);
					break;
				case 1:
					app->render->DrawTexture(gritoGuerra, (400 + 200 * i) / scale, 525 / scale);
					break;
				case 2:
					app->render->DrawTexture(laserGun, (400 + 200 * i) / scale, 525 / scale);
					break;
				case 3:
					app->render->DrawTexture(fireBall, (400 + 200 * i) / scale, 525 / scale);
					break;
				case 4:
					app->render->DrawTexture(healingMagic, (400 + 200 * i) / scale, 525 / scale);
					break;
				case 5:
					app->render->DrawTexture(laserCannon, (400 + 200 * i) / scale, 525 / scale);
					break;
				case 6:
					app->render->DrawTexture(martillazo, (400 + 200 * i) / scale, 525 / scale);
					break;
				}

			}

			currentArrowAnim->Update();
			app->render->DrawTexture(arrowTexture, (450 + 200 * selectAttackIndex) / scale, 500 / scale, &currentArrowAnim->GetCurrentFrame());


			//Navigate in the selection attack menu
			if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || pad.l_x > 0 && selectActionCooldown == 0)
			{
				if (selectAttackIndex < app->scene->allies[currentPlayerInCombatIndex]->abilities.Count() - 1)
				{
					selectAttackIndex += 1;

					app->audio->PlayFx(hoverFx);
				}
				selectActionCooldown = 10;
				
			}
			if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || pad.l_x < 0 && selectActionCooldown == 0)
			{
				if (selectAttackIndex > 0)
				{
					selectAttackIndex -= 1;

					app->audio->PlayFx(hoverFx);
				}
				selectActionCooldown = 10;
				
			}
			//Selected action
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || pad.a && selectActionCooldown == 0)
			{
				selectActionCooldown = 10;

				currentEnemySelectedIndex = FindFirstEnemyIndex();
				app->scene->allies[currentPlayerInCombatIndex]->CheckAttack(app->scene->allies[currentPlayerInCombatIndex]->abilities[selectAttackIndex].id, currentPlayerInCombatIndex);

				app->audio->PlayFx(clickFx);
			}
			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.b && selectActionCooldown == 0)
			{
				printf("vuelves a la seleccion de personaje\n");
				currentPlayerInCombatIndex = FindFirstPlayerToAttackIndex();
				combatState = CombatState::SELECT_CHARACTER;

				app->audio->PlayFx(declineFx);
				selectActionCooldown = 10;
			}
			break;
		case CombatState::SELECT_ENEMY:
			//Navigate in the selection enemy menu
			if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.l_y > 0 && selectActionCooldown == 0)
			{
				if (currentEnemySelectedIndex + 1 < app->scene->enemies.Count() && app->scene->enemies[currentEnemySelectedIndex + 1]->life > 0)
				{

					app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = false;
					currentEnemySelectedIndex += 1;
					app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = true;

					app->audio->PlayFx(hoverFx);
				}
				selectActionCooldown = 10;
				
			}
			if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.l_y < 0 && selectActionCooldown == 0)
			{
				if (currentEnemySelectedIndex - 1 >= 0 && app->scene->enemies[currentEnemySelectedIndex - 1]->life > 0)
				{
					app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = false;
					currentEnemySelectedIndex -= 1;
					app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = true;

					app->audio->PlayFx(hoverFx);
				}
				selectActionCooldown = 10;
				
			}
			//Selected character, waiting for action
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || pad.a && selectActionCooldown == 0)
			{
				selectActionCooldown = 10;
				if (app->scene->enemies[currentEnemySelectedIndex]->life > 0)
				{
					app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = false;
					app->scene->allies[currentPlayerInCombatIndex]->SetAttackAnimation();
					switch (idAttack)
					{
					case 0:
						app->scene->allies[currentPlayerInCombatIndex]->hasAttacked = true;
						damage = app->scene->allies[currentPlayerInCombatIndex]->attack / app->scene->enemies[currentEnemySelectedIndex]->defense * 20;
						app->scene->enemies[currentEnemySelectedIndex]->life -= damage;
						isText = true;
						break;
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						app->scene->allies[currentPlayerInCombatIndex]->hasAttacked = true;
						damage = app->scene->allies[currentPlayerInCombatIndex]->magicPower * 0.75f;
						printf("El Ca�on laser ha hecho %f de da�o\n", damage);
						app->scene->enemies[currentEnemySelectedIndex]->life -= damage;
						isText = true;
						break;
					case 6:
						app->scene->allies[currentPlayerInCombatIndex]->hasAttacked = true;
						damage = app->scene->allies[currentPlayerInCombatIndex]->attack / app->scene->enemies[currentEnemySelectedIndex]->defense * 20;
						app->scene->enemies[currentEnemySelectedIndex]->life -= damage;
						isText = true;
						break;
					}

				}
				else
				{
					printf("that enemy is dead, please select another\n");

					app->audio->PlayFx(deniedFx);
				}
				if (CheckAllPlayersAttacked()) {
					CheckState();
					app->battleScene->combatState = CombatState::ENEMY_ATTACK;
					
					selectAttackIndex = 0;
				}
				else
				{
					currentPlayerInCombatIndex = FindFirstPlayerToAttackIndex();
					CheckState();
					app->battleScene->combatState = CombatState::SELECT_CHARACTER;
					
					selectAttackIndex = 0;
				}

				app->audio->PlayFx(clickFx);
			}
			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.b && selectActionCooldown == 0)
			{
				printf("vuelves a la seleccion de personaje\n");
				currentPlayerInCombatIndex = FindFirstPlayerToAttackIndex();
				combatState = CombatState::SELECT_CHARACTER;

				app->audio->PlayFx(declineFx);
				selectActionCooldown = 10;

			}

			//CheckState();
			//app->render->DrawCircle((app->scene->enemies[currentEnemySelectedIndex]->position.x + 100) / scale, (app->scene->enemies[currentEnemySelectedIndex]->position.y + 40) / scale, 5, 255, 0, 0, 255);
			currentArrowAnim->Update();
			app->render->DrawTexture(arrowTexture, (app->scene->enemies[currentEnemySelectedIndex]->position.x + 30) / scale, (app->scene->enemies[currentEnemySelectedIndex]->position.y) / scale, &currentArrowAnim->GetCurrentFrame());


			break;
		case CombatState::ENEMY_ATTACK:
			//Aqui va el ataque del enemigo y despues un check State
			timerEnemy++;
			srand((unsigned)time(NULL));
			int indexAttack = rand() % app->scene->allies.Count();

			if (app->scene->enemies[currentEnemyInCombatIndex]->life <= 0)
			{
				currentEnemyInCombatIndex++;
			}

			if (timerEnemy >= 120)
			{
				app->scene->enemies[currentEnemyInCombatIndex]->SetAttackAnimation();
				damage = app->scene->enemies[currentEnemyInCombatIndex]->attack / app->scene->allies[indexAttack]->defense * 20;
				if (app->physics->debug == false)
				{
					app->scene->allies[indexAttack]->life -= damage;
				}
				
				printf("Ataque de enemigo a %s y le ha hecho %f de da�o\n", app->scene->allies[indexAttack]->charName.GetString(), damage);
				printf("La vida de %s es: %f\n", app->scene->allies[indexAttack]->charName.GetString(), app->scene->allies[indexAttack]->life);
				timerEnemy = 0;
				currentEnemyInCombatIndex++;
			}

			if (currentEnemyInCombatIndex == app->scene->enemies.Count())
			{
				CheckState();
				currentEnemyInCombatIndex = 0;
				SetAllPlayersAliveToAttack();
				currentPlayerInCombatIndex = FindFirstPlayerToAttackIndex();
				combatState = CombatState::SELECT_CHARACTER;
			}

			break;

		}

		if (isText)
		{

			if (timerText <= 120)
			{
				TextAttack(idAttack);
				timerText++;
			}
			else {
				timerText = 0;
				idAttack = 0;
				isText = false;
			}

		}
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
		app->dialogueManager->playerHasLosed = true;
		app->scene->Enable();
		for (int i = 0; i < app->scene->npcs.Count(); ++i)
		{
			if (app->scene->npcs[i]->npcID == npcIDbattle)
			{
				app->scene->npcs[i]->hasTalked = false;
				app->scene->npcIDcombatFinished.Add(npcIDbattle);
			}
		}
		Disable();
		app->scene->isOnCombat = false;
		for (int i = 0; i < app->scene->allies.Count(); i++)
		{
			app->scene->allies[i]->life = 0;
		}
		for (int i = 0; i < app->scene->enemies.Count(); i++)
		{
			app->scene->enemies[i]->life = 0;
		}
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
		for (int i = 0; i < app->scene->npcs.Count(); ++i)
		{
			if (app->scene->npcs[i]->npcID == npcIDbattle)
			{
				app->scene->npcs[i]->hasCombat = false;
				app->scene->npcs[i]->hasTalked = true;
				app->scene->npcIDcombatFinished.Add(app->scene->npcs[i]->npcID);
			}
		}
		for (int i = 0; i < app->scene->allies.Count(); i++)
		{
			app->scene->allies[i]->life = 0;
		}
		for (int i = 0; i < app->scene->enemies.Count(); i++)
		{
			app->scene->enemies[i]->life = 0;
		}
		Disable();
		app->scene->isOnCombat = false;
		hasStartedCombat = false;
		return;
	}
}

int BattleScene::FindFirstEnemyIndex() 
{
	for (int i = 0; i < app->scene->enemies.Count(); i++)
	{
		if (app->scene->enemies[i]->life > 0) {
			return i;
		}
	}
	return -1;

}

int BattleScene::FindFirstPlayerToAttackIndex() 
{
	for (int i = 0; i < app->scene->allies.Count(); i++)
	{
		if (app->scene->allies[i]->life > 0 && !app->scene->allies[i]->hasAttacked) {
			return i;
		}
	}
	return -1;

}

bool BattleScene::CheckAllPlayersAttacked() 
{
	for (int i = 0; i < app->scene->allies.Count(); i++)
	{
		if (!app->scene->allies[i]->hasAttacked) {
			return false;
		}
	}
	return true;

}

void BattleScene::SetAllPlayersAliveToAttack() {
	for (int i = 0; i < app->scene->allies.Count(); i++)
	{
		if (app->scene->allies[i]->life>0) {
			app->scene->allies[i]->hasAttacked = false;
		}
	}
}

void BattleScene::TextAttack(int indexAttack)
{
	switch (indexAttack)
	{
	case 0:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "comete mi sable!!");
		break;
	case 1:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "aumenta mi ataque WROAAAAR!!");
		break;
	case 2:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "pium pium!! Visión Laser!!");
		break;
	case 3:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "fireball!!");
		break;
	case 4:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "diosa curame!!");
		break;
	case 5:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "bomba laseeer!!");
		break;
	case 6:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "ja ja ja martillo vaa!!");
		break;
	}
}






