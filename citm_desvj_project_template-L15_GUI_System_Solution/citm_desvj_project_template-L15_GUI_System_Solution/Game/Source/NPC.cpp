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

NPC::NPC() : Entity(EntityType::NPC)
{
	name.Create("npc");
}

NPC::~NPC() {}

bool NPC::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	dialogueChar = parameters.attribute("dialogue").as_string();
	dialogueString = dialogueChar;

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

	pbody = app->physics->CreateRectangle(position.x, position.y, 60, 60, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;

	return true;
}

bool NPC::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		isTalking = !isTalking;
	}

	pbody->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x)), PIXEL_TO_METERS((float32)(position.y)) }, 0);
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool NPC::CleanUp()
{
	return true;
}

void NPC::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER)
	{
		if (isTalking)
		{
			app->dialogueManager->CreateDialogue(dialogueString, DialogueType::NPC);
		}
		
	}
}