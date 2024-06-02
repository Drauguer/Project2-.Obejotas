#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "MainMenu.h"
#include "Map.h"
#include "Item.h"
#include "NPC.h"
#include "ModuleFonts.h"
#include "ModuleFadeToBlack.h"
#include "Scene.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "DialogueManager.h"

MainMenu::MainMenu(bool startEnabled) : Module(startEnabled)
{
	name.Create("MainMenu");
}

// Destructor
MainMenu::~MainMenu()
{}

bool MainMenu::Awake(pugi::xml_node config)
{
	for (pugi::xml_node node = config.child("menuAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		menuAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });

	}
	menuAnim.speed = config.child("menuAnim").attribute("animspeed").as_float();
	menuAnim.loop = config.child("menuAnim").attribute("loop").as_bool();
	texturePath = config.attribute("texturePath").as_string();


	return true;
}

bool MainMenu::Start()
{

	img = app->tex->Load(texturePath);



	currentAnimation = &menuAnim;



	hoverFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/001Hover01.wav");
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/013Confirm03.wav");
	declineFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/029Decline09.wav");
	deniedFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/033Denied03.wav");

	IdleStartButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalStart.png");
	IdleContinueButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalContinue.png");
	IdleOptionsButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalOptions.png");
	IdleExitButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalExit.png");

	IdleBackButton = app->tex->Load("Assets/Textures/BotonesUI/SettingsPrincipalBack.png");
	IdleFullScreenButton = app->tex->Load("Assets/Textures/BotonesUI/SettingsPrincipalMaximizar.png");
	IdleNormalScreenButton = app->tex->Load("Assets/Textures/BotonesUI/SettingsPrincipalMinimizar.png");
	IdleVsyncButton = app->tex->Load("Assets/Textures/BotonesUI/SettingsPrincipalVsync.png");

	ActiveStartButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalStartHighlighted.png");
	ActiveContinueButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalContinueHighlighted.png");
	ActiveOptionsButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalOptionsHighlighted.png");
	ActiveExitButton = app->tex->Load("Assets/Textures/BotonesUI/MenuPrincipalExitHighlighted.png");

	ActiveBackButton = app->tex->Load("Assets/Textures/BotonesUI/SettingsReturnHighlighted.png");
	ActiveFullScreenButton = app->tex->Load("Assets/Textures/BotonesUI/SettingsMaximisesHighlighted.png");
	ActiveNormalScreenButton = app->tex->Load("Assets/Textures/BotonesUI/SettingsMininizesHighlighted.png");
	ActiveVsyncButton = app->tex->Load("Assets/Textures/BotonesUI/SettingsVsyncHighlighted.png");

	app->win->GetWindowSize(windowW, windowH);
	





	SDL_Rect StartButton = { windowW / 2 - 400,windowH / 2 +100, 240,80 };
	start = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Start", StartButton, this);

	SDL_Rect ExitButton = { windowW / 2 - 400,windowH / 2 + 240, 240, 80 };
	exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton, this);

	SDL_Rect SettingsButton = { windowW / 2 - 0,windowH / 2 +240, 240, 80 };
	setting = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", SettingsButton, this);

	SDL_Rect GoBackButton = { windowW / 2 + 260,windowH / 2 +240, 240, 80 };
	turnBack = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Go Back", GoBackButton, this);
	turnBack->state = GuiControlState::DISABLED;

	SDL_Rect ContinueButton = { windowW / 2 +0, windowH / 2 + 100, 240, 80 };
	continue_ = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Continue", ContinueButton, this);

	SDL_Rect FullScreenCheck = { windowW / 2 -400,windowH / 2 + 80, 240, 80 };
	FullScreen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Full Screen", FullScreenCheck, this);
	FullScreen->state = GuiControlState::DISABLED;

	SDL_Rect FullScreenCheckOff = { windowW / 2 - 210,windowH / 2 - 240, 240, 80 };
	FullScreenOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Full Screen Off", FullScreenCheckOff, this);
	FullScreenOff->state = GuiControlState::DISABLED;

	SDL_Rect VsincCheck = { windowW / 2 + 260,windowH / 2 + 80 , 240, 80 };
	Vsinc = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "Vsync", VsincCheck, this);
	Vsinc->state = GuiControlState::DISABLED;

	SDL_Rect VsincCheckOff = { windowW / 2 + 260,windowH / 2 + 140 , 240, 80 };
	VsincOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "Vsync Off", VsincCheckOff, this);
	VsincOff->state = GuiControlState::DISABLED;

	SDL_Rect SoundOff = { windowW / 2 +260,windowH / 2 - 80,240,80 };
	AudioOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "Audio Off", SoundOff, this);
	AudioOff->state = GuiControlState::DISABLED;

	SDL_Rect SoundOn = { windowW / 2 +260,windowH / 2 - 140,240,80 };
	AudioOn = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "Audio On", SoundOn, this);
	AudioOn->state = GuiControlState::DISABLED;

	SDL_Rect EffectOff = { windowW / 2 + 260,windowH / 2 + -260,240,80 };
	FxOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "Fx Off", EffectOff, this);
	FxOff->state = GuiControlState::DISABLED;

	SDL_Rect EffectOn = { windowW / 2 +260,windowH / 2 -320,240,80 };
	FxOn = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "Fx On", EffectOn, this);
	FxOn->state = GuiControlState::DISABLED;

	app->audio->PlayMusic("Assets/Audio/Music/Music Loops/TitleMusic.ogg", 2.0f);

	return true;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{


	currentAnimation->Update();
	app->render->DrawTexture(img, textPosX, textPosY, &currentAnimation->GetCurrentFrame(), false);

	GamePad& pad = app->input->pads[0];

	if (!isOnSettings) {
		//Fullscreen
		//Exit
		//Fx
		//Audio
		//Vsync 
		//Go back

		if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.l_y > 0 && app->selectActionCooldown == 0) && uiGamePadCounter < 3)
		{
			uiGamePadCounter++;
			app->selectActionCooldown = 20;

		}
		if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.l_y < 0 && app->selectActionCooldown == 0) && uiGamePadCounter > 0)
		{
			uiGamePadCounter--;
			app->selectActionCooldown = 20;

		}
		if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || pad.l_x > 0 && app->selectActionCooldown == 0) && uiGamePadCounter < 2)
		{
			uiGamePadCounter += 2;
			app->selectActionCooldown = 20;

		}
		if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || pad.l_x < 0 && app->selectActionCooldown == 0) && uiGamePadCounter > 1)
		{
			uiGamePadCounter -= 2;
			app->selectActionCooldown = 20;

		}
		start->isButtonPressed = false;
		continue_->isButtonPressed = false;
		exit->isButtonPressed = false;
		setting->isButtonPressed = false;
		switch (uiGamePadCounter)
		{
		case 0:
			start->isButtonPressed = true;
			break;
		case 1:
			exit->isButtonPressed = true;
			break;
		case 2:
			continue_->isButtonPressed = true;
			break;
		case 3:
			setting->isButtonPressed = true;
			break;

		default:
			break;
		}
	}
	else
	{
		if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.l_y > 0 && app->selectActionCooldown == 0) && uiGamePadCounter < 6)
		{
			uiGamePadCounter++;
			app->selectActionCooldown = 20;

		}
		if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.l_y < 0 && app->selectActionCooldown == 0) && uiGamePadCounter > 0)
		{
			uiGamePadCounter--;
			app->selectActionCooldown = 20;

		}
		if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || pad.l_x > 0 && app->selectActionCooldown == 0) && uiGamePadCounter < 2)
		{
			if (uiGamePadCounter == 0)
			{
				uiGamePadCounter += 2;
			}
			else
			{
				uiGamePadCounter += 4;
			}
			app->selectActionCooldown = 20;

		}
		if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || pad.l_x < 0 && app->selectActionCooldown == 0) && uiGamePadCounter > 1)
		{
			if (uiGamePadCounter==2) 
			{
				uiGamePadCounter -= 2;

			}
			else if (uiGamePadCounter == 3)
			{
				uiGamePadCounter -= 3;
			}
			else
			{
				uiGamePadCounter -= 4;
			}
			app->selectActionCooldown = 20;

		}
		FullScreen->isButtonPressed = false;
		FullScreenOff->isButtonPressed = false;
		exit->isButtonPressed = false;
		FxOn->isButtonPressed = false;
		FxOff->isButtonPressed = false;
		AudioOn->isButtonPressed = false;
		AudioOff->isButtonPressed = false;
		AudioOff->isButtonPressed = false;
		Vsinc->isButtonPressed = false;
		VsincOff->isButtonPressed = false;
		turnBack->isButtonPressed = false;
		switch (uiGamePadCounter)
		{
		case 0:
			if (FullScreen->state == GuiControlState::DISABLED) {
				FullScreenOff->isButtonPressed = true;
			}
			else
			{
				FullScreen->isButtonPressed = true;
			}
			break;
		case 1:
			exit->isButtonPressed = true;
			break;
		case 2:
			if (FxOn->state == GuiControlState::DISABLED) {
				FxOff->isButtonPressed = true;
			}
			else
			{
				FxOn->isButtonPressed = true;
			}
			break;
		case 3:
			if (AudioOn->state == GuiControlState::DISABLED) {
				AudioOff->isButtonPressed = true;
			}
			else
			{
				AudioOn->isButtonPressed = true;
			}
			break;
		case 4:
			if (Vsinc->state == GuiControlState::DISABLED) {
				VsincOff->isButtonPressed = true;
			}
			else
			{
				Vsinc->isButtonPressed = true;
			}
			break;
		case 5:
			turnBack->isButtonPressed = true;
			break;

		default:
			break;
		}
	}

	if (app->selectActionCooldown > 0) {
		app->selectActionCooldown--;
	}
	
	if (isExiting == true)
	{
		return false;
	}

	return true;
}

bool MainMenu::PostUpdate()
{
	return true;
}

bool MainMenu::CleanUp()
{
	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	

	if (control->id == 1) {

		app->fadeToBlack->FadeToBlack(this, app->scene, 30);
		app->particleSystem->Enable();
		app->physics->Enable();
		app->map->Enable();
		app->entityManager->Enable();
		start->state = GuiControlState::DISABLED;
		continue_->state = GuiControlState::DISABLED;
		setting->state = GuiControlState::DISABLED;
		FullScreen->state = GuiControlState::DISABLED;
		exit->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}

	if (control->id == 2) {
		isExiting = true;
	}

	if (control->id == 3) {
		isOnSettings = true;
		uiGamePadCounter = 0;
		start->state = GuiControlState::DISABLED;
		continue_->state = GuiControlState::DISABLED;
		setting->state = GuiControlState::DISABLED;
		turnBack->state = GuiControlState::NORMAL;
		Vsinc->state = GuiControlState::NORMAL;
		FullScreen->state = GuiControlState::NORMAL;
		AudioOff->state = GuiControlState::NORMAL;
		FxOff->state = GuiControlState::NORMAL;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 6) {
		isOnSettings = false;
		uiGamePadCounter = 0;
		start->state = GuiControlState::NORMAL;
		continue_->state = GuiControlState::NORMAL;
		setting->state = GuiControlState::NORMAL;
		turnBack->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::DISABLED;
		Vsinc->state = GuiControlState::DISABLED;
		FullScreen->state = GuiControlState::DISABLED;
		FullScreenOff->state = GuiControlState::DISABLED;
		AudioOff->state = GuiControlState::DISABLED;
		AudioOn->state = GuiControlState::DISABLED;
		FxOff->state = GuiControlState::DISABLED;
		FxOn->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 10) {
		app->particleSystem->Enable();
		app->physics->Enable();
		app->map->Enable();
		app->entityManager->Enable();
		app->scene->Enable();
		app->LoadRequest();
		this->Disable();

		exit->state = GuiControlState::DISABLED;
		start->state = GuiControlState::DISABLED;
		setting->state = GuiControlState::DISABLED;
		continue_->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 11) {
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
		FullScreenOff->state = GuiControlState::NORMAL;
		FullScreen->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 12) {
		app->win->GetWindowSize(windowW, windowH);
		SDL_SetWindowSize(app->win->window, windowW, windowH);
		SDL_SetWindowFullscreen(app->win->window, 0);

		FullScreen->state = GuiControlState::NORMAL;
		FullScreenOff->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 13) {
		app->IsVsincActive = false;
		Vsinc->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::NORMAL;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 14) {
		app->IsVsincActive = true;
		Vsinc->state = GuiControlState::NORMAL;
		VsincOff->state = GuiControlState::DISABLED;

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 15) {
		AudioOff->state = GuiControlState::DISABLED;
		AudioOn->state = GuiControlState::NORMAL;
		app->audio->SetMusicVolume(0.0f);
		
	}
	if (control->id == 16) {
		AudioOff->state = GuiControlState::NORMAL;
		AudioOn->state = GuiControlState::DISABLED;
		app->audio->SetMusicVolume(100);

		app->audio->PlayFx(clickFx);
	}
	if (control->id == 17) {
		FxOff->state = GuiControlState::DISABLED;
		FxOn->state = GuiControlState::NORMAL;
		app->audio->SetFxVolume(0.0f);

		app->audio->PlayFx(clickFx);

	}
	if (control->id == 18) {
		FxOff->state = GuiControlState::NORMAL;
		FxOn->state = GuiControlState::DISABLED;
		app->audio->SetFxVolume(100);

		app->audio->PlayFx(clickFx);
	}
	

	return true;
}

void MainMenu::Enable()
{
	if (!active)
	{
		active = true;

		Start();
	}
}

void MainMenu::Disable()
{
	if (active)
	{
		active = false;
		CleanUp();
	}
}