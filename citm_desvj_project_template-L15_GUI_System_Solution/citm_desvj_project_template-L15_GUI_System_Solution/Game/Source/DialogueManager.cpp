#include "DialogueManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

DialogueManager::DialogueManager() : Module()
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

	item = dialogueList.start;

	return true;
}

bool DialogueManager::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		testDialogue = !testDialogue;
	}

	if (testDialogue)
	{
		SDL_Rect dialogueBoxPos = { windowW / 2 - 600, windowH / 2 + 120, 1200, 250};
		app->render->DrawRectangle(dialogueBoxPos, 0, 50, 255, 255);

		if (item != NULL && ret == true)
		{
			app->render->DrawText(item->data->text.GetString(), dialogueBoxPos.x + 15, dialogueBoxPos.y + 15, 1170, 40);

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				item = item->next;
				
				if (item == NULL)
				{
					testDialogue = false;
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


