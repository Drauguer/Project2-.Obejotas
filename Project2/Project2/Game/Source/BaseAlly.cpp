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

BaseAlly::BaseAlly() : Entity(EntityType::ALLY)
{
	name.Create("base ally");
	abilities.Add({ 1, "Fireball" });
	abilities.Add({ 2, "Potion" });
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

bool BaseAlly::Awake() 
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	life = parameters.attribute("life").as_int();
	attack = parameters.attribute("attack").as_int();

	//InitDialogues();

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

	return true;
}

bool BaseAlly::Update(float dt)
{
	if (life > 0) {
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x)), PIXEL_TO_METERS((float32)(position.y)) }, 0);
		app->render->DrawTexture(texture, position.x, position.y);
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
	switch (selectAttackIndex)
	{
	case 0:
		printf("Bola de Fuego\n");
		app->battleScene->combatState = CombatState::SELECT_ENEMY;
		break;
	case 1:
		printf("Curación +10 de vida\n");
		app->scene->allies[currentPlayerIndex]->life += 10;
		app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		break;
	}
}