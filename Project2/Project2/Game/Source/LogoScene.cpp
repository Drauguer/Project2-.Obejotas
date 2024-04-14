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
#include "LogoScene.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "DialogueManager.h"

LogoScene::LogoScene(bool startEnabled) : Module(startEnabled)
{
	name.Create("mainmenu");
}

// Destructor
LogoScene::~LogoScene()
{}

bool LogoScene::Awake(pugi::xml_node config)
{
	return true;
}

bool LogoScene::Start()
{

	img = app->tex->Load("Assets/Textures/logo.png");
	timer = 0;


	return true;
}

bool LogoScene::PreUpdate()
{
	return true;
}

bool LogoScene::Update(float dt)
{


	timer += dt;
	app->render->DrawTexture(img, textPosX, textPosY, false);
	if (timer > 3000 || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->mainMenu->Enable();
		this->Disable();
	}

	return true;
}

bool LogoScene::PostUpdate()
{
	return true;
}

bool LogoScene::CleanUp()
{
	return true;
}

bool LogoScene::OnGuiMouseClickEvent(GuiControl* control)
{
	
	return true;
}

void LogoScene::Enable()
{
	if (!active)
	{
		active = true;

		Start();
	}
}

void LogoScene::Disable()
{
	if (active)
	{
		active = false;
		CleanUp();
	}
}