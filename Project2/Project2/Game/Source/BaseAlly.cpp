#include "BaseAlly.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "BattleScene.h"
#include "Window.h"

BaseAlly::BaseAlly() : Entity(EntityType::ALLY)
{
	name.Create("base ally");
}

BaseAlly::~BaseAlly() {}

void BaseAlly::InitDialogues()
{
	// Load all the dialogues
	for (pugi::xml_node node = parameters.child("dialogue"); node; node = node.next_sibling("dialogue")) {
		dialogueChar = node.attribute("text").as_string();
		dialogueString = dialogueChar;
		dialoguesNPC.Add(dialogueString);
	}

}

void BaseAlly::InitAnims()
{
	// Idle
	for (pugi::xml_node node = parameters.child("Idle").child("pushback"); node; node = node.next_sibling("pushback")) {
		Idle.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Idle.speed = parameters.child("Idle").attribute("animspeed").as_float();
	Idle.loop = parameters.child("Idle").attribute("loop").as_bool();

}

bool BaseAlly::Awake() 
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	life = parameters.attribute("life").as_int();
	attack = parameters.attribute("attack").as_int();
	defense = parameters.attribute("defense").as_int();
	magicPower = parameters.attribute("magicPower").as_int();
	dexerity = parameters.attribute("dexerity").as_int();
	charName = parameters.attribute("charName").as_string();


	for (pugi::xml_node node = parameters.child("ability"); node; node = node.next_sibling("ability")) 
	{
		abilityId = node.attribute("id").as_int();
		abilityName = node.attribute("name").as_string();
		char* abilityString = const_cast<char*>(abilityName);
		abilities.Add({ abilityId, abilityString });
		numAttacks++;
	}
	

	//InitDialogues();
	InitAnims();

	return true;
}

bool BaseAlly::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	int npc[8] = {
		-60, -60,
		60, -60,
		60, 60,
		-60, 60,
	};

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 60, 60, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;

	currentAnim = &Idle;

	return true;
}

bool BaseAlly::Update(float dt)
{

	int scale = app->win->GetScale();

	if (life > 0) {
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x / scale)), PIXEL_TO_METERS((float32)(position.y / scale)) }, 0);
		app->render->DrawTexture(texture, position.x / scale, position.y / scale, &currentAnim->GetCurrentFrame());
	}
	

	return true;
}

bool BaseAlly::CleanUp()
{
	return true;
}

void BaseAlly::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

bool BaseAlly::OnCollisionStay(PhysBody* physA, PhysBody* physB)
{
	int xA, yA, xB, yB;

	physA->GetPosition(xA, yA);
	physB->GetPosition(xB, yB);

	if (xB <= xA + (physA->width * 2) && xB >= xA && yB >= yA && yB <= yA + (physA->height * 2))
	{
		return true;
	}

	return false;
}

// Mira que ataque tiene que hacer el personaje
void BaseAlly::CheckAttack(int selectAttackIndex, int currentPlayerIndex)
{
	//Here we check if we have to select an enemy or just make the effect 

	switch (selectAttackIndex)
	{
	case 0:
		printf("Bola de Fuego\n");
		hasAttacked = true;
		app->battleScene->combatState = CombatState::SELECT_ENEMY;
		break;
	case 1:
		hasAttacked = true;
		printf("Curación +10 de vida\n");
		app->scene->allies[currentPlayerIndex]->life += 10;
		if (app->battleScene->CheckAllPlayersAttacked()) {
			app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		}
		else
		{
			app->battleScene->currentPlayerInCombatIndex = app->battleScene->FindFirstPlayerToAttackIndex();
			app->battleScene->combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case 2:
		hasAttacked = true;
		printf("Sword Attack!!\n");
		app->battleScene->combatState = CombatState::SELECT_ENEMY;
		break;
	}
}