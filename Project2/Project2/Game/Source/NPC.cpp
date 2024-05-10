#include "NPC.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"

NPC::NPC() : Entity(EntityType::NPC)
{
	name.Create("npc");
}

NPC::~NPC() {}

void NPC::InitDialogues()
{
	// Load all the dialogues
	for (pugi::xml_node node = parameters.child("dialogue"); node; node = node.next_sibling("dialogue")) {
		dialogueChar = node.attribute("text").as_string();
		dialogueString = dialogueChar;
		dialoguesNPC.Add(dialogueString);
	}
	
}

bool NPC::Awake() {

	

	return true;
}

bool NPC::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	hasCombat = parameters.attribute("hasCombat").as_bool();
	triggerInRange = parameters.attribute("triggerInRange").as_bool();
	mapID = parameters.attribute("mapID").as_int();
	npcID = parameters.attribute("npcID").as_int();
	hasQuest = parameters.attribute("hasQuest").as_bool();
	QuestID = parameters.attribute("QuestID").as_int();
	itemRewardID = parameters.attribute("itemRewardID").as_int();

	for (pugi::xml_node node = parameters.child("idleAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		idleAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	idleAnim.speed = parameters.child("idleAnim").attribute("animspeed").as_float();
	idleAnim.loop = parameters.child("idleAnim").attribute("loop").as_bool();

	for (int i = 0; i < app->scene->npcIDcombatFinished.Count(); ++i)
	{
		if (app->scene->npcIDcombatFinished[i] == npcID)
		{
			hasCombat = false;
			hasTalked = true;
		}
	}

	InitDialogues();

	//initilize textures
	texture = app->tex->Load(texturePath);

	int npc[8] = {
		-60, -60,
		60, -60,
		60, 60,
		-60, 60,
	};

	pbody = app->physics->CreateRectangleSensor(position.x / 2 + 40, position.y / 2 + 40, 50, 50, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;

	return true;
}

bool NPC::Update(float dt)
{

	int scale = app->win->GetScale();
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	GamePad& pad = app->input->pads[0];

	if (triggerInRange)
	{
		if (OnCollisionStay(this->pbody, app->scene->player->pbody) && app->dialogueManager->isTalking == false && hasTalked == false)
		{

			app->scene->player->isTalking = true;

			app->dialogueManager->isTalking = true;
			hasTalked = true;
			app->dialogueManager->testDialogue = !app->dialogueManager->testDialogue;
			LOG("Dialogue Start");

			if (hasQuest && QuestCompleted == false && cantTalk == false)
			{
				CheckQuest(QuestID);
			}

			ListItem<SString>* item;

			for (item = dialoguesNPC.start; item != NULL; item = item->next)
			{
				app->dialogueManager->CreateDialogue(item->data, DialogueType::NPC);
			}

			if (hasCombat)
			{
				app->dialogueManager->activateCombat = true;
				app->dialogueManager->npcIDcombat = npcID;

			}

			
		}

		if (cantTalk && app->scene->player->isTalking == false)
		{
			timerTalk++;

			if (timerTalk > 20)
			{
				cantTalk = false;
				hasTalked = false;
				timerTalk = 0;
			}
		}
	}
	else if (!triggerInRange)
	{
		if (OnCollisionStay(this->pbody, app->scene->player->pbody) && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.y && app->dialogueManager->isTalking == false)
		{

			app->scene->player->isTalking = true;

			app->dialogueManager->isTalking = true;
			app->dialogueManager->testDialogue = !app->dialogueManager->testDialogue;
			LOG("Dialogue Start");

			if (hasQuest && QuestCompleted == false)
			{
				CheckQuest(QuestID);
			}

			ListItem<SString>* item;

			for (item = dialoguesNPC.start; item != NULL; item = item->next)
			{
				app->dialogueManager->CreateDialogue(item->data, DialogueType::NPC);
			}

			if (hasCombat)
			{
				app->dialogueManager->activateCombat = true;
				app->dialogueManager->npcIDcombat = npcID;
			}

			

		}

		

	}


	pbody->body->SetTransform({ PIXEL_TO_METERS((float32)((position.x + 40) / scale)), PIXEL_TO_METERS((float32)((position.y + 40) / scale)) }, 0);
	if (!app->scene->isOnCombat) {
		app->render->DrawTexture(texture, (position.x - 25) / scale, (position.y - 25) / scale, &currentAnimation->GetCurrentFrame());
	}

	return true;
}

bool NPC::CleanUp()
{
	return true;
}

void NPC::OnCollision(PhysBody* physA, PhysBody* physB)
{
	
}

bool NPC::OnCollisionStay(PhysBody* physA, PhysBody* physB) 
{
	int scale = app->win->GetScale();

	int xA, yA, xB, yB;

	physA->GetPosition(xA, yA);
	physB->GetPosition(xB, yB);

	if (xB <= xA + (physA->width) && xB >= xA && yB >= yA && yB <= yA + (physA->height))
	{
		return true;
	}

	return false;
}


bool NPC::SaveState(pugi::xml_node node, int num)
{
	SString childName("NPC%d", num);


	pugi::xml_node npc = node.append_child(childName.GetString());

	pugi::xml_node pos = npc.append_child("Position");
	pugi::xml_attribute x = pos.append_attribute("x");
	pugi::xml_attribute y = pos.append_attribute("y");
	pugi::xml_node state = npc.append_child("State");
	pugi::xml_attribute combat = state.append_attribute("Has_Combat");
	pugi::xml_attribute talked = state.append_attribute("Has_Talked");


	x.set_value(pbody->body->GetPosition().x);
	y.set_value(pbody->body->GetPosition().y);
	combat.set_value(hasCombat);
	talked.set_value(hasTalked);
	return true;
}


bool NPC::LoadState(pugi::xml_node node, int num)
{
	SString childName("NPC%d", num);


	pugi::xml_node npc = node.child(childName.GetString());
	pugi::xml_node pos = npc.child("Position");
	float32 x = pos.attribute("x").as_float();
	float32 y = pos.attribute("y").as_float();
	pugi::xml_node state = npc.child("State");
	
	pbody->body->SetTransform(b2Vec2(x, y), 0);
	hasCombat = state.attribute("Has_Combat").as_bool();
	hasTalked = state.attribute("Has_Talked").as_bool();

	return true;


}

void NPC::CheckQuest(int questID)
{
	switch (questID)
	{
	case 0:
			for (int j = 0; j < app->scene->allies[0]->inventoryChar.Count(); ++j)
			{
				if (strcmp(app->scene->allies[0]->inventoryChar[j]->itemName.GetString(), "Mage Item Quest") == 0)
				{
					QuestCompleted = true;
					// Add Mage to the allies
					for (pugi::xml_node allyNode = app->scene->scene_parameter.child("mage"); allyNode; allyNode = allyNode.next_sibling("mage"))
					{
						BaseAlly* ally = (BaseAlly*)app->entityManager->CreateEntity(EntityType::ALLY);
						app->scene->allies.Add(ally);
						ally->parameters = allyNode;
						ally->Start();

					}

					// Load the dialogues after completing the quest
					for (pugi::xml_node node = parameters.child("dialogueQuest"); node; node = node.next_sibling("dialogueQuest")) {
						dialogueChar = node.attribute("text").as_string();
						dialogueString = dialogueChar;
						dialoguesNPC.Add(dialogueString);
					}

					// Add the item to the mage inventory
					for (int k = 0; k < app->scene->allies.Count(); ++k)
					{
						if (strcmp(app->scene->allies[k]->charName.GetString(), "Gale el Mago") == 0)
						{
							app->scene->allies[1]->inventoryChar.Add(app->scene->allies[0]->inventoryChar[j]);
							
							ListItem<Item*>* item = app->scene->allies[0]->inventoryChar.start;

							for (int h = 0; h < app->scene->allies[0]->inventoryChar.Count(); ++h)
							{
								if (strcmp(item->data->itemName.GetString(), "Mage Item Quest") != 0)
								{
									item = item->next;
								}
							}
							app->scene->allies[0]->inventoryChar.Del(item);
						}
					}
					

				}
			}
		break;
	case 1:
		for (int k = 0; k < app->scene->allies.Count(); ++k)
		{
			if (strcmp(app->scene->allies[k]->charName.GetString(), "Gale el Mago") == 0)
			{
				QuestCompleted = true;

				// Load the dialogues after completing the quest
				for (pugi::xml_node node = parameters.child("dialogueQuest"); node; node = node.next_sibling("dialogueQuest")) {
					dialogueChar = node.attribute("text").as_string();
					dialogueString = dialogueChar;
					dialoguesNPC.Add(dialogueString);
				}

				// Add Enano to the allies
				for (pugi::xml_node allyNode = app->scene->scene_parameter.child("enano"); allyNode; allyNode = allyNode.next_sibling("enano"))
				{
					BaseAlly* ally = (BaseAlly*)app->entityManager->CreateEntity(EntityType::ALLY);
					app->scene->allies.Add(ally);
					ally->parameters = allyNode;
					ally->Start();

				}
			}
		}
		break;
	case 2:
		if (app->scene->allies.Count() == 4)
		{
			hasCombat = true;

			QuestCompleted = true;

			// Load the dialogues after completing the quest
			for (pugi::xml_node node = parameters.child("dialogueQuest"); node; node = node.next_sibling("dialogueQuest")) {
				dialogueChar = node.attribute("text").as_string();
				dialogueString = dialogueChar;
				dialoguesNPC.Add(dialogueString);
			}
		}
		else
		{
			cantTalk = true;
		}
		break;
	}
}