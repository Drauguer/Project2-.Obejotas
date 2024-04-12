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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	hasCombat = parameters.attribute("hasCombat").as_bool();
	triggerInRange = parameters.attribute("triggerInRange").as_bool();
	mapID = parameters.attribute("mapID").as_int();

	InitDialogues();

	return true;
}

bool NPC::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	int npc[8] = {
		-60, -60,
		60, -60,
		60, 60,
		-60, 60,
	};

	pbody = app->physics->CreateRectangleSensor(position.x / 2, position.y / 2, 80, 80, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;

	return true;
}

bool NPC::Update(float dt)
{

	int scale = app->win->GetScale();

	GamePad& pad = app->input->pads[0];

	if (triggerInRange)
	{
		if (OnCollisionStay(this->pbody, app->scene->player->pbody) && app->dialogueManager->isTalking == false && hasTalked == false)
		{

			app->dialogueManager->isTalking = true;
			hasTalked = true;
			app->dialogueManager->testDialogue = !app->dialogueManager->testDialogue;
			LOG("Dialogue Start");

			ListItem<SString>* item;

			for (item = dialoguesNPC.start; item != NULL; item = item->next)
			{
				app->dialogueManager->CreateDialogue(item->data, DialogueType::NPC);
			}

			if (hasCombat)
			{
				app->dialogueManager->activateCombat = true;
			}

		}
	}
	else if (!triggerInRange)
	{
		if (OnCollisionStay(this->pbody, app->scene->player->pbody) && app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN || pad.y && app->dialogueManager->isTalking == false)
		{

			app->dialogueManager->isTalking = true;
			app->dialogueManager->testDialogue = !app->dialogueManager->testDialogue;
			LOG("Dialogue Start");

			ListItem<SString>* item;

			for (item = dialoguesNPC.start; item != NULL; item = item->next)
			{
				app->dialogueManager->CreateDialogue(item->data, DialogueType::NPC);
			}

			if (hasCombat)
			{
				app->dialogueManager->activateCombat = true;
			}

		}
	}

	

	

	pbody->body->SetTransform({ PIXEL_TO_METERS((float32)((position.x + 130) / scale)), PIXEL_TO_METERS((float32)((position.y + 130) / scale)) }, 0);
	app->render->DrawTexture(texture, position.x / scale, position.y / scale);

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