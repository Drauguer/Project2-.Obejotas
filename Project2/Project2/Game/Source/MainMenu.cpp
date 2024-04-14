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
	name.Create("mainmenu");
}

// Destructor
MainMenu::~MainMenu()
{}

bool MainMenu::Awake(pugi::xml_node config)
{
	return true;
}

bool MainMenu::Start()
{
	app->win->GetWindowSize(windowW, windowH);
	img = app->tex->Load("Assets/Textures/main_screen.png");

	SDL_Rect StartButton = { windowW / 2 - 300,windowH / 2 +100, 240,80 };
	start = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Start", StartButton, this);

	SDL_Rect ExitButton = { windowW / 2 - 300,windowH / 2 + 240, 240, 80 };
	exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton, this);

	SDL_Rect SettingsButton = { windowW / 2 - 0,windowH / 2 +240, 240, 80 };
	setting = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", SettingsButton, this);

	SDL_Rect GoBackButton = { windowW / 2 - 0,windowH / 2 - 40, 240, 80 };
	turnBack = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Go Back", GoBackButton, this);
	turnBack->state = GuiControlState::DISABLED;

	SDL_Rect ContinueButton = { windowW / 2 +0, windowH / 2 + 100, 240, 80 };
	continue_ = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Continue", ContinueButton, this);

	SDL_Rect FullScreenCheck = { windowW / 2 - 0,windowH / 2 -140, 240, 80 };
	FullScreen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Full Screen", FullScreenCheck, this);
	FullScreen->state = GuiControlState::DISABLED;

	SDL_Rect FullScreenCheckOff = { windowW / 2 - 210,windowH / 2 - 240, 240, 80 };
	FullScreenOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Full Screen Off", FullScreenCheckOff, this);
	FullScreenOff->state = GuiControlState::DISABLED;

	SDL_Rect VsincCheck = { windowW / 2 - 450,windowH / 2 - 120 , 240, 80 };
	Vsinc = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "Vsinc", VsincCheck, this);
	Vsinc->state = GuiControlState::DISABLED;

	SDL_Rect VsincCheckOff = { windowW / 2 - 210,windowH / 2 - 120 , 240, 80 };
	VsincOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "Vsinc Off", VsincCheckOff, this);
	VsincOff->state = GuiControlState::DISABLED;

	app->audio->PlayMusic("Assets/Audio/Music/Music Loops/TitleMusic.ogg", 2.0f);

	return true;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{
	GamePad& pad = app->input->pads[0];

	app->render->DrawTexture(img, textPosX, textPosY, false);
	

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

		app->physics->Enable();
		app->map->Enable();
		app->entityManager->Enable();
		app->scene->Enable();
		this->Disable();
		start->state = GuiControlState::DISABLED;
		continue_->state = GuiControlState::DISABLED;
		setting->state = GuiControlState::DISABLED;
		FullScreen->state = GuiControlState::DISABLED;
		exit->state = GuiControlState::DISABLED;
	}

	if (control->id == 2) {
		app->scene->IsExiting == true;
	}

	if (control->id == 3) {
		start->state = GuiControlState::DISABLED;
		continue_->state = GuiControlState::DISABLED;
		setting->state = GuiControlState::DISABLED;
		turnBack->state = GuiControlState::NORMAL;
		Vsinc->state = GuiControlState::NORMAL;
		FullScreen->state = GuiControlState::NORMAL;
	}
	if (control->id == 6) {
		start->state = GuiControlState::NORMAL;
		continue_->state = GuiControlState::NORMAL;
		setting->state = GuiControlState::NORMAL;
		turnBack->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::DISABLED;
		Vsinc->state = GuiControlState::DISABLED;
	}
	if (control->id == 10) {

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
	}
	if (control->id == 11) {
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		FullScreenOff->state = GuiControlState::NORMAL;
		FullScreen->state = GuiControlState::DISABLED;
	}
	if (control->id == 12) {
		app->win->GetWindowSize(windowW, windowH);
		SDL_SetWindowSize(app->win->window, windowW, windowH);
		SDL_SetWindowFullscreen(app->win->window, 0);

		FullScreen->state = GuiControlState::NORMAL;
		FullScreenOff->state = GuiControlState::DISABLED;
	}
	if (control->id == 13) {
		app->IsVsincActive = false;
		Vsinc->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::NORMAL;
	}
	if (control->id == 14) {
		app->IsVsincActive = true;
		Vsinc->state = GuiControlState::NORMAL;
		VsincOff->state = GuiControlState::DISABLED;
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