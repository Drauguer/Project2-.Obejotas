#include "DialogueManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Window.h"
#include "ModuleFonts.h"
#include "BattleScene.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

DialogueManager::DialogueManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("dialoguemanager");
}

// Destructor
DialogueManager::~DialogueManager()
{}

bool DialogueManager::Awake(pugi::xml_node config)
{


	return true;
}

bool DialogueManager::Start()
{
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz 1234567890.,'=(?!)+-*/      " };
	Font = app->fonts->Load("Assets/Fonts/typography.png", lookupTable, 2);

	item = dialogueList.start;

	return true;
}

bool DialogueManager::Update(float dt)
{
	bool ret = true;

	GamePad& pad = app->input->pads[0];

	int scale = app->win->GetScale();

	if (testDialogue)
	{
		item = dialogueList.start;
		

		if (item != NULL && ret == true)
		{
			SDL_Rect dialogueBoxPos = { (windowW / 2 - 600) / scale, (windowH / 2 + 120) / scale, 1200 / scale, 250 / scale };
			app->render->DrawRectangle(dialogueBoxPos, 0, 50, 255, 255);
			app->fonts->BlitText(dialogueBoxPos.x + 15, dialogueBoxPos.y + 15, Font,item->data->text.GetString());
			LOG("Write Text");

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a)
			{
				dialogueList.Del(item);
				item = item->next;
				LOG("Next dialogue");
				
				if (item == NULL)
				{
					testDialogue = false;
					isTalking = false;
					item = dialogueList.start;
					
					if (activateCombat)
					{
						app->scene->isOnCombat = !app->scene->isOnCombat;
						app->scene->Disable();
						app->battleScene->Enable();
						app->audio->PlayFx(app->scene->encounterFx);
						activateCombat = false;
					}
				}
			}
		}
	}

	return ret;
}

bool DialogueManager::CleanUp()
{


	return true;
}

Dialogue* DialogueManager::CreateDialogue(SString _text, DialogueType type)
{

	Dialogue* dialogue = new Dialogue();

	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case DialogueType::PLAYER:
		dialogue->text = _text;
		break;
	case DialogueType::ITEM:
		dialogue->text = _text;
		break;
	case DialogueType::NPC:
		dialogue->text = _text;
		break;
	case DialogueType::ENEMY:
		dialogue->text = _text;
		break;
	default:
		break;
	}

	dialogueList.Add(dialogue);

	return dialogue;

}

void DialogueManager::DestroyDialogue(Dialogue* dialogue)
{

}

void DialogueManager::AddDialogue(Dialogue* dialogue)
{
	if (dialogue != nullptr) dialogueList.Add(dialogue);
}


