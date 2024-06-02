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
#include "EndScreen.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "DialogueManager.h"

EndScreen::EndScreen(bool startEnabled) : Module(startEnabled)
{
	name.Create("EndScreen");
}

// Destructor
EndScreen::~EndScreen()
{}

bool EndScreen::Awake(pugi::xml_node config)
{
	

	return true;
}

bool EndScreen::Start()
{

	img = app->tex->Load("Assets/Textures/EndScreen.png");
	timer = 0;

	currentAnimation = &logoAnim;

	app->audio->PlayMusic("Assets/Audio/Music/Music Loops/EndingMusic.ogg", 0.5f);

	return true;
}

bool EndScreen::PreUpdate()
{
	return true;
}

bool EndScreen::Update(float dt)
{



	// Increment timer based on delta time (frame rate independent)
	timer += dt;

	currentAnimation->Update();
	app->render->DrawTexture(img, 200, 150, false);

	

	return true;
}

bool EndScreen::PostUpdate()
{
	return true;
}

bool EndScreen::CleanUp()
{
	return true;
}

bool EndScreen::OnGuiMouseClickEvent(GuiControl* control)
{

	return true;
}

void EndScreen::Enable()
{
	if (!active)
	{
		active = true;

		Start();
	}
}

void EndScreen::Disable()
{
	if (active)
	{
		active = false;
		CleanUp();
	}
}