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
	app->render->DrawCircle(300, 300, 50, 0, 255, 0, 255);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.start)
	{
		app->physics->Enable();
		app->map->Enable();
		app->entityManager->Enable();
		app->scene->Enable();
		this->Disable();
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