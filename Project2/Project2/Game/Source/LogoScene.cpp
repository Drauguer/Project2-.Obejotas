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
	name.Create("logoScene");
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

	app->audio->PlayMusic("Assets/Audio/Music/ScreenMusic/TeamLogoSFX.ogg", 1.0f);

	return true;
}

bool LogoScene::PreUpdate()
{
	return true;
}

bool LogoScene::Update(float dt)
{

	app->render->DrawTexture(img, textPosX, textPosY, false);

	// Increment timer based on delta time (frame rate independent)
	timer += dt;

	// Check if it's time to trigger the fade effect
	if (timer > 200 )
	{
		// Trigger fade effect
		app->fadeToBlack->FadeToBlack(this, app->mainMenu, 30);
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