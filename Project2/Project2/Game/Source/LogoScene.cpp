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
	for (pugi::xml_node node = config.child("logoAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		logoAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	logoAnim.speed = config.child("logoAnim").attribute("animspeed").as_float();
	logoAnim.loop = config.child("logoAnim").attribute("loop").as_bool();
	texturePath = config.attribute("texturePath").as_string();

	return true;
}

bool LogoScene::Start()
{

	img = app->tex->Load(texturePath);
	timer = 0;

	currentAnimation = &logoAnim;
	
	app->audio->PlayMusic("Assets/Audio/Music/ScreenMusic/TeamLogoSFX.ogg", 1.0f);

	return true;
}

bool LogoScene::PreUpdate()
{
	return true;
}

bool LogoScene::Update(float dt)
{

	

	// Increment timer based on delta time (frame rate independent)
	timer += dt;

	currentAnimation->Update();
	app->render->DrawTexture(img, textPosX, textPosY, &currentAnimation->GetCurrentFrame(), false);

	// Check if it's time to trigger the fade effect
	if (timer > 1200 )
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