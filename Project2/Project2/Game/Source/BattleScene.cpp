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
#include "ModuleFadeToBlack.h"

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


	arrowTexturePath = config.attribute("arrowTexturePath").as_string();
	winTexturePath = config.attribute("winTexturePath").as_string();
	loseTexturePath = config.attribute("loseTexturePath").as_string();

	for (pugi::xml_node node = config.child("arrow").child("pushback"); node; node = node.next_sibling("pushback")) {
		idleArrowAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	idleArrowAnim.speed = config.child("arrow").attribute("animspeed").as_float();
	idleArrowAnim.loop = config.child("arrow").attribute("loop").as_bool();
	
	for (pugi::xml_node node = config.child("win").child("pushback"); node; node = node.next_sibling("pushback")) {
		winAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	winAnim.speed = config.child("win").attribute("animspeed").as_float();
	winAnim.loop = config.child("win").attribute("loop").as_bool();
	
	for (pugi::xml_node node = config.child("lose").child("pushback"); node; node = node.next_sibling("pushback")) {
		loseAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	loseAnim.speed = config.child("lose").attribute("animspeed").as_float();
	loseAnim.loop = config.child("lose").attribute("loop").as_bool();

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


	//if player has entered boss room, play boss music
	if (app->scene->hasEnteredBoss)
	{
		app->audio->PlayMusic("Assets/Audio/Music/Music Loops/FinalBossBattleTheme.ogg");
	}
	else
	{
		app->audio->PlayMusic("Assets/Audio/Music/Music Loops/BattleTheme.ogg");
	}
	

	sableLaser = app->tex->Load("Assets/Textures/LaserSwords.png");
	gritoGuerra = app->tex->Load("Assets/Textures/Scream_Icon.png");
	laserGun = app->tex->Load("Assets/Textures/lasergun_Icon.png");
	fireBall = app->tex->Load("Assets/Textures/FireBall_Icon.png");
	healingMagic = app->tex->Load("Assets/Textures/MagicHealth_Icon.png");
	laserCannon = app->tex->Load("Assets/Textures/laserbeam_Icon.png");
	martillazo = app->tex->Load("Assets/Textures/RedMace.png");
	elMuro = app->tex->Load("Assets/Textures/EnanoShield_Icon.png");
	lluviaAsesina = app->tex->Load("Assets/Textures/RedMace.png");
	inspiracionBardica = app->tex->Load("Assets/Textures/SatyrAttack1_Icon.png");
	vivaRock = app->tex->Load("Assets/Textures/SatyrAttack2_Icon.png");
	musicaMaldita = app->tex->Load("Assets/Textures/SatyrAttack3_Icon.png");



	lifeMago = app->tex->Load("Assets/Textures/LifeMago.png");
	lifeEnano = app->tex->Load("Assets/Textures/LifeEnano.png");
	lifeEldrin = app->tex->Load("Assets/Textures/LifeEldrin.png");
	lifeBardo = app->tex->Load("Assets/Textures/LifeBardo.png");
	lifeEnemy = app->tex->Load("Assets/Textures/LifeEnemy.png");
	IdleContinueButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalContinue.png");
	IdleExitButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalExit.png");
	background = app->tex->Load("Assets/Maps/FondoCombate.png");

	arrowTexture = app->tex->Load(arrowTexturePath);
	winTexture = app->tex->Load(winTexturePath);
	loseTexture = app->tex->Load(loseTexturePath);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	SDL_Rect ContinueButton_ = { windowW / 2 + 60,windowH / 2 + 120, 240, 80 };
	continueBattle = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Continue", ContinueButton_, this, IdleContinueButton);
	continueBattle->state = GuiControlState::DISABLED;

	SDL_Rect ExitButton_ = { windowW / 2 - 360,windowH / 2 + 120, 240, 80 };
	exitBattle = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton_, this, IdleExitButton);
	exitBattle->state = GuiControlState::DISABLED;
	
	

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
		dialogueBoxPos = { (winW / 2 - 600 - app->render->camera.x) / scale, (winH / 2 + 170 - app->render->camera.y) / scale, 1200 / scale, 250 / scale };
		dialogueBoxPos2 = { (winW / 2 - 600) / scale, (winH / 2 + 170) / scale, 1200 / scale, 250 / scale };

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
			else if (strcmp(app->scene->allies[i]->charName.GetString(), "Eldrin") == 0)
			{
				app->render->DrawRectangle(app->scene->allies[i]->healthBar, 0, 255, 0, 255);
				app->render->DrawTexture(lifeEldrin, (app->scene->allies[i]->position.x - 150) / scale, app->scene->allies[i]->position.y / scale);

			}
			else if (strcmp(app->scene->allies[i]->charName.GetString(), "Urgo el Bardo") == 0)
			{
				app->render->DrawRectangle(app->scene->allies[i]->healthBar, 0, 255, 0, 255);
				app->render->DrawTexture(lifeBardo, (app->scene->allies[i]->position.x - 150) / scale, app->scene->allies[i]->position.y / scale);

			}
		}

		for (int i = 0; i < app->scene->enemies.Count(); ++i)
		{
			app->render->DrawRectangle(app->scene->enemies[i]->healthBar, 0, 255, 0, 255);
			app->render->DrawTexture(lifeEnemy, (app->scene->enemies[i]->position.x + 100) / scale, app->scene->enemies[i]->position.y / scale);
		}

		currentArrowAnim = &idleArrowAnim;
		//Timer for waiting to select action
		if (app->selectActionCooldown > 0) {
			app->selectActionCooldown--;
		}
		//Initial values
		if (!hasStartedCombat)
		{
			CheckState();
			combatState = CombatState::SELECT_CHARACTER;
			currentPlayerInCombatIndex = FindFirstPlayerIndex();
			app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = true;
			app->scene->allies[currentPlayerInCombatIndex]->isHighlighted = false;
			hasStartedCombat = true;
		}
		//Combat state
		if (!hasEndedCombat)
		{
			switch (combatState)
			{
			case CombatState::SELECT_CHARACTER:
				//Navigate in the selection character menu
				if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.l_y > 0 && app->selectActionCooldown == 0)
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
					app->selectActionCooldown = 10;
				
				}
				if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.l_y < 0 && app->selectActionCooldown == 0)
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
					app->selectActionCooldown = 10;
				
				}
				//Selected character, waiting for action
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a && app->selectActionCooldown == 0)
				{
					app->selectActionCooldown = 10;
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
					case 7:
						app->render->DrawTexture(elMuro, (400 + 200 * i) / scale, 525 / scale);
						break;
					case 8:
						app->render->DrawTexture(sableLaser, (400 + 200 * i) / scale, 525 / scale);
						break;
					case 9:
						app->render->DrawTexture(lluviaAsesina, (400 + 200 * i) / scale, 525 / scale);
						break;
					case 10:
						app->render->DrawTexture(inspiracionBardica, (400 + 200 * i) / scale, 525 / scale);
						break;
					case 11:
						app->render->DrawTexture(vivaRock, (400 + 200 * i) / scale, 525 / scale);
						break;
					case 12:
						app->render->DrawTexture(musicaMaldita, (400 + 200 * i) / scale, 525 / scale);
						break;
					}

				}

				currentArrowAnim->Update();
				app->render->DrawTexture(arrowTexture, (450 + 200 * selectAttackIndex) / scale, 500 / scale, &currentArrowAnim->GetCurrentFrame());


				//Navigate in the selection attack menu
				if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || pad.l_x > 0 && app->selectActionCooldown == 0)
				{
					if (selectAttackIndex < app->scene->allies[currentPlayerInCombatIndex]->abilities.Count() - 1)
					{
						selectAttackIndex += 1;

						app->audio->PlayFx(hoverFx);
					}
					app->selectActionCooldown = 10;
				
				}
				if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || pad.l_x < 0 && app->selectActionCooldown == 0)
				{
					if (selectAttackIndex > 0)
					{
						selectAttackIndex -= 1;

						app->audio->PlayFx(hoverFx);
					}
					app->selectActionCooldown = 10;
				
				}
				//Selected action
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a && app->selectActionCooldown == 0)
				{
					app->selectActionCooldown = 10;

					currentEnemySelectedIndex = FindFirstEnemyIndex();
					app->scene->allies[currentPlayerInCombatIndex]->CheckAttack(app->scene->allies[currentPlayerInCombatIndex]->abilities[selectAttackIndex].id, currentPlayerInCombatIndex);

					app->audio->PlayFx(clickFx);
				}
				if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.b && app->selectActionCooldown == 0)
				{
					printf("vuelves a la seleccion de personaje\n");
					currentPlayerInCombatIndex = FindFirstPlayerToAttackIndex();
					combatState = CombatState::SELECT_CHARACTER;

					app->audio->PlayFx(declineFx);
					app->selectActionCooldown = 10;
				}
				break;
			case CombatState::SELECT_ENEMY:
				//Navigate in the selection enemy menu
				if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.l_y > 0 && app->selectActionCooldown == 0)
				{
					if (currentEnemySelectedIndex + 1 < app->scene->enemies.Count() && app->scene->enemies[currentEnemySelectedIndex + 1]->life > 0)
					{

						app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = false;
						currentEnemySelectedIndex += 1;
						app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = true;

						app->audio->PlayFx(hoverFx);
					}
					app->selectActionCooldown = 10;
				
				}
				if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.l_y < 0 && app->selectActionCooldown == 0)
				{
					if (currentEnemySelectedIndex - 1 >= 0 && app->scene->enemies[currentEnemySelectedIndex - 1]->life > 0)
					{
						app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = false;
						currentEnemySelectedIndex -= 1;
						app->scene->enemies[currentEnemySelectedIndex]->isHighlighted = true;

						app->audio->PlayFx(hoverFx);
					}
					app->selectActionCooldown = 10;
				
				}
				//Selected character, waiting for action
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a && app->selectActionCooldown == 0)
				{
					app->selectActionCooldown = 10;
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
						case 7:
							break;
						case 8:
							app->scene->allies[currentPlayerInCombatIndex]->hasAttacked = true;
							damage = app->scene->allies[currentPlayerInCombatIndex]->attack / app->scene->enemies[currentEnemySelectedIndex]->defense * 20;
							app->scene->enemies[currentEnemySelectedIndex]->life -= damage;
							isText = true;
							break;
						case 9:
							break;
						case 10:
							break;
						case 11:
							break;
						case 12:
							app->scene->allies[currentPlayerInCombatIndex]->hasAttacked = true;
							damage = app->scene->allies[currentPlayerInCombatIndex]->magicPower * 0.5f;
							printf("El ataque ha hecho %f de da�o y le ha bajado la defensa\n", damage);
							app->scene->enemies[currentEnemySelectedIndex]->life -= damage;
							app->scene->enemies[currentEnemySelectedIndex]->defense *= 0.5f;
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
				if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.b && app->selectActionCooldown == 0)
				{
					printf("vuelves a la seleccion de personaje\n");
					currentPlayerInCombatIndex = FindFirstPlayerToAttackIndex();
					combatState = CombatState::SELECT_CHARACTER;

					app->audio->PlayFx(declineFx);
					app->selectActionCooldown = 10;

				}

				//CheckState();
				//app->render->DrawCircle((app->scene->enemies[currentEnemySelectedIndex]->position.x + 100) / scale, (app->scene->enemies[currentEnemySelectedIndex]->position.y + 40) / scale, 5, 255, 0, 0, 255);
				currentArrowAnim->Update();
				app->render->DrawTexture(arrowTexture, (app->scene->enemies[currentEnemySelectedIndex]->position.x + 30) / scale, (app->scene->enemies[currentEnemySelectedIndex]->position.y) / scale, &currentArrowAnim->GetCurrentFrame());


				break;
			case CombatState::ENEMY_ATTACK:
				//Aqui va el ataque del enemigo y despues un check State
			
				//Aqui se podria hacer una animacion para seleccionar al player al que va a atacar, 
				//por ejemplo la flechita moviendose rapidamente entre players hasta que selecciona a uno 
				timerEnemy++;
				srand((unsigned)time(NULL));
				int indexPlayerToAttack = rand() % app->scene->allies.Count();
				int indexAbility = rand() % app->scene->enemies[currentEnemyInCombatIndex]->abilities.Count();

				if (app->scene->enemies[currentEnemyInCombatIndex]->life <= 0)
				{
					currentEnemyInCombatIndex++;
				}

				if (timerEnemy >= 120)
				{
					app->scene->enemies[currentEnemyInCombatIndex]->SetAttackAnimation();
					CheckEnemyAbility(indexAbility, indexPlayerToAttack);			
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
		else
		{
			if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || pad.l_x > 0 && app->selectActionCooldown == 0) && uiGamePadCounter < 1)
			{
				uiGamePadCounter ++;
				app->selectActionCooldown = 20;

			}
			if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || pad.l_x < 0 && app->selectActionCooldown == 0) && uiGamePadCounter > 0)
			{
				uiGamePadCounter --;
				app->selectActionCooldown = 20;

			}
			continueBattle->isButtonPressed = false;
			exitBattle->isButtonPressed = false;
			switch (uiGamePadCounter)
			{
			case 0:
				exitBattle->isButtonPressed = true;
				break;
			case 1:
				continueBattle->isButtonPressed = true;
				break;
			default:
				break;
			}
			int winLoseWidth;
			if (playerLose) 
			{
				currentFinalAnimation = &loseAnim;
				winLoseWidth = 448;
				app->render->DrawTexture(loseTexture, (windowW - winLoseWidth) / 4, (windowH - 300) / 4, &currentFinalAnimation->GetCurrentFrame());

			}
			else
			{
				currentFinalAnimation = &winAnim;
				winLoseWidth = 392;
				app->render->DrawTexture(winTexture, (windowW - winLoseWidth) / 4, (windowH - 300) / 4, &currentFinalAnimation->GetCurrentFrame());


			}
			currentFinalAnimation->Update();

		}
		

	}


	return true;
}

// Called each loop iteration
bool BattleScene::PostUpdate()
{
	bool ret = true;
	if (isExiting == true)
		ret = false;
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

	if (control->id == 1) 
	{
		app->audio->PlayFx(clickFx);
		exitBattle->state = GuiControlState::DISABLED;
		continueBattle->state = GuiControlState::DISABLED;
		hasStartedCombat = false;
		hasEndedCombat = false;
		currentFinalAnimation->Reset();
		if (playerLose) {
			PlayerLose();

		}
		else
		{
			PlayerWin();
		}
		
	}
	else
	{
		app->audio->PlayFx(clickFx);
		isExiting = true;
	}

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
	for (int i = 0; i < app->scene->enemies.Count(); i++)
	{
		if (app->scene->enemies[i]->life > 0)
		{
			playerWin = false;
			break;
		}
	}
	if (playerWin || playerLose) {
		hasEndedCombat = true;
		exitBattle->state = GuiControlState::NORMAL;
		continueBattle->state = GuiControlState::NORMAL;
	}
}

void BattleScene::PlayerLose()
{
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
	app->scene->isOnCombat = false;
	for (int i = 0; i < app->scene->allies.Count(); i++)
	{
		app->scene->allies[i]->life = app->scene->allies[i]->maxHP;
	}
	
	
	Disable();
	return;
}
void BattleScene::PlayerWin()
{
	printf("Has ganado el combate");
	app->fadeToBlack->FadeToBlack(app->battleScene, app->scene, 20);
	for (int i = 0; i < app->scene->npcs.Count(); ++i)
	{
		if (app->scene->npcs[i]->npcID == npcIDbattle)
		{
			app->scene->npcs[i]->hasCombat = false;
			app->scene->npcs[i]->hasTalked = true;
			app->scene->npcIDcombatFinished.Add(app->scene->npcs[i]->npcID);
			GiveReward(app->scene->npcs[i]->npcID);
		}
	}

	app->scene->isOnCombat = false;
	return;
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
int BattleScene::FindFirstPlayerIndex() 
{
	for (int i = 0; i < app->scene->allies.Count(); i++)
	{
		if (app->scene->allies[i]->life > 0 ) {
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
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "wroarrrrrr!! (+atk)");
		break;
	case 2:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "pium pium!! get shot!!");
		break;
	case 3:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "fireball!!");
		break;
	case 4:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "godess heal me!!");
		break;
	case 5:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "bombaaa!!");
		break;
	case 6:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "hammer time!!");
		break;
	case 7:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "im immovable! (+def)");
		break;
	case 8:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "this sword will seal your sin");
		break;
	case 9:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "succumb under my rain!");
		break;
	case 10:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "don't worry, be happy! (+hp y +atk)");
		break;
	case 11:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "rock and roll!!");
		break;
	case 12:
		app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
		app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "this song will give you nighmares");
		break;
	}
}

//Function to check the enemy hability based on the id 
void BattleScene::CheckEnemyAbility(int abilityId, int indexPlayerToAttack) {
	switch (abilityId)
	{
	case 0:
		damage = app->scene->enemies[currentEnemyInCombatIndex]->attack / app->scene->allies[indexPlayerToAttack]->defense * 20;
		printf("Ataque de enemigo a %s y le ha hecho %f de da�o\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), damage);
		printf("La vida de %s es: %f\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), app->scene->allies[indexPlayerToAttack]->life);
		if (app->physics->debug == false)
		{
			app->scene->allies[indexPlayerToAttack]->life -= damage;
		}
		break;
	case 1:
		if (currentEnemyInCombatIndex + 1 < app->scene->enemies.Count())
		{
			app->scene->enemies[currentEnemyInCombatIndex + 1]->life -= 10;
			app->scene->enemies[currentEnemyInCombatIndex]->life += 25;
			if (app->scene->enemies[currentEnemyInCombatIndex]->life >= app->scene->enemies[currentEnemyInCombatIndex]->maxHP)
			{
				app->scene->enemies[currentEnemyInCombatIndex]->life = app->scene->enemies[currentEnemyInCombatIndex]->maxHP;
			}

			printf("Daño a su amigo y se cura\n");
		}
		
		break;
	case 2:
		damage = app->scene->enemies[currentEnemyInCombatIndex]->attack / app->scene->allies[indexPlayerToAttack]->defense * 20;
		printf("Ataque de enemigo a %s y le ha hecho %f de da�o\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), damage);
		printf("La vida de %s es: %f\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), app->scene->allies[indexPlayerToAttack]->life);
		if (app->physics->debug == false)
		{
			app->scene->allies[indexPlayerToAttack]->life -= damage;
		}

		printf("Placaje XD\n");
		break;
	case 3:
		damage = app->scene->enemies[currentEnemyInCombatIndex]->magicPower - 10;
		printf("Ataque de enemigo a %s y le ha hecho %f de da�o\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), damage);
		printf("La vida de %s es: %f\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), app->scene->allies[indexPlayerToAttack]->life);
		if (app->physics->debug == false)
		{
			app->scene->allies[indexPlayerToAttack]->life -= damage;
		}

		printf("Blob Attack\n");
		break;
	case 4:
		damage = (app->scene->enemies[currentEnemyInCombatIndex]->attack / app->scene->allies[indexPlayerToAttack]->defense * 20) * 0.5f;
		for (int i = 0; i < app->scene->allies.Count(); ++i)
		{
			srand((unsigned)time(NULL));
			randomIndex = rand() % 100;

			if (app->physics->debug == false && (randomIndex <= app->scene->enemies[currentEnemyInCombatIndex]->dexerity))
			{
				app->scene->allies[i]->life -= damage;
				printf("Ataque de enemigo a %s y le ha hecho %f de da�o\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), damage);
				printf("La vida de %s es: %f\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), app->scene->allies[indexPlayerToAttack]->life);
			}
		}

		printf("Disparos laser\n");

		break;
	case 5:
		damage = app->scene->enemies[currentEnemyInCombatIndex]->attack * 2 / app->scene->allies[indexPlayerToAttack]->defense * 20;
		

		srand((unsigned)time(NULL));
		randomIndex = rand() % 100;

		if (app->physics->debug == false && (randomIndex <= app->scene->enemies[currentEnemyInCombatIndex]->dexerity))
		{
			app->scene->allies[indexPlayerToAttack]->life -= damage;
			printf("Ataque de enemigo a %s y le ha hecho %f de da�o\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), damage);
			printf("La vida de %s es: %f\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), app->scene->allies[indexPlayerToAttack]->life);
		}

		printf("Cañon\n");
		break;
	case 6:
		app->scene->enemies[currentEnemyInCombatIndex]->life += app->scene->enemies[currentEnemyInCombatIndex]->maxHP / 2;
		if (app->scene->enemies[currentEnemyInCombatIndex]->life >= app->scene->enemies[currentEnemyInCombatIndex]->maxHP)
		{
			app->scene->enemies[currentEnemyInCombatIndex]->life = app->scene->enemies[currentEnemyInCombatIndex]->maxHP;
		}

		printf("Se cura la mitad de vida\n");
		break;
	case 7:
		damage = app->scene->enemies[currentEnemyInCombatIndex]->attack * 2 / app->scene->allies[indexPlayerToAttack]->defense * 20;

		srand((unsigned)time(NULL));
		randomIndex = rand() % 100;

		if (app->physics->debug == false && (randomIndex <= app->scene->enemies[currentEnemyInCombatIndex]->dexerity))
		{
			app->scene->allies[indexPlayerToAttack]->life -= damage;
			printf("Ataque de enemigo a %s y le ha hecho %f de da�o\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), damage);
			printf("La vida de %s es: %f\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), app->scene->allies[indexPlayerToAttack]->life);
		}

		printf("Cañonazo\n");
		break;
	case 8:
		damage = (app->scene->enemies[currentEnemyInCombatIndex]->attack / app->scene->allies[indexPlayerToAttack]->defense * 20) * 0.75f;
		for (int i = 0; i < app->scene->allies.Count(); ++i)
		{
			srand((unsigned)time(NULL));
			randomIndex = rand() % 100;

			if (app->physics->debug == false && (randomIndex <= app->scene->enemies[currentEnemyInCombatIndex]->dexerity))
			{
				app->scene->allies[i]->life -= damage;
				printf("Ataque de enemigo a %s y le ha hecho %f de da�o\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), damage);
				printf("La vida de %s es: %f\n", app->scene->allies[indexPlayerToAttack]->charName.GetString(), app->scene->allies[indexPlayerToAttack]->life);
			}
		}

		printf("Disparos laser\n");
		break;
	case 9:
		app->scene->enemies[currentEnemyInCombatIndex]->life += app->scene->enemies[currentEnemyInCombatIndex]->maxHP / 3;
		if (app->scene->enemies[currentEnemyInCombatIndex]->life >= app->scene->enemies[currentEnemyInCombatIndex]->maxHP)
		{
			app->scene->enemies[currentEnemyInCombatIndex]->life = app->scene->enemies[currentEnemyInCombatIndex]->maxHP;
		}

		printf("Se cura un tercio de vida\n");
		break;
	}
}

void BattleScene::GiveReward(int npcID)
{
	switch (npcID)
	{
	case 5:
		app->scene->allies[0]->inventoryChar.Add(app->scene->itemMage);
		app->scene->itemMage->newInInventory = true;
		break;
	case 8:
		app->scene->allies[0]->inventoryChar.Add(app->scene->itemQuest3);
		app->scene->itemQuest3->newInInventory = true;
		break;
	case 11:
		app->scene->allies[0]->inventoryChar.Add(app->scene->itemNum1);
		app->scene->itemNum1->newInInventory = true;
		break;
	case 12:
		app->scene->allies[0]->inventoryChar.Add(app->scene->itemNum2);
		app->scene->itemNum2->newInInventory = true;
		break;
	case 13:
		app->scene->allies[0]->inventoryChar.Add(app->scene->itemNum3);
		app->scene->itemNum3->newInInventory = true;
		break;
	}
}








