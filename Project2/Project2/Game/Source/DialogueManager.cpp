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
#include "ModuleFadeToBlack.h"

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
	chatbox = app->tex->Load("Assets/Textures/Chatbox.png");

	item = dialogueList.start;

	dialogueClickFx = app->audio->LoadFx("Assets/Audio/Fx/10_UI_Menu_SFX/001Hover01.wav");

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
			SDL_Rect dialogueBoxPos = { (windowW / 2 - 600 - app->render->camera.x) / scale, (windowH / 2 + 120 - app->render->camera.y) / scale, 1200 / scale, 250 / scale };
			SDL_Rect dialogueBoxPos2 = { (windowW / 2 - 600) / scale, (windowH / 2 + 120) / scale, 1200 / scale, 250 / scale };
			//app->render->DrawRectangle(dialogueBoxPos, 0, 50, 255, 255);
			app->render->DrawTexture(chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
			app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, Font,item->data->text.GetString());
			LOG("Write Text");

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || (pad.a&&app->selectActionCooldown==0))
			{
				app->audio->PlayFx(dialogueClickFx);
				app->selectActionCooldown = 20;
				dialogueList.Del(item);
				item = item->next;
				LOG("Next dialogue");
				
				if (item == NULL)
				{
					testDialogue = false;
					isTalking = false;
					app->scene->player->isTalking = false;
					app->scene->player->pbody->body->SetTransform(b2Vec2(app->scene->player->pbody->body->GetTransform().p.x, app->scene->player->pbody->body->GetTransform().p.y + 0.5f), 0);
					item = dialogueList.start;

					
					
					if (activateCombat)
					{
						app->scene->isOnCombat = !app->scene->isOnCombat;
						if (app->scene->isOnCombat) {
							app->particleSystem->Disable();
						}
						app->battleScene->npcIDbattle = npcIDcombat;
						app->scene->combatID = npcIDcombat;
						if (firstCombat)
						{
							//app->scene->LoadAllies();
							firstCombat = false;
						}
						if (!playerHasLosed)
						{
							
							app->scene->LoadEnemies();
							playerHasLosed = false;
						}
						app->fadeToBlack->FadeToBlack(app->scene, app->battleScene, 20);
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


