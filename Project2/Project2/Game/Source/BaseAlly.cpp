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

#include<iostream>
#include<cstdlib>

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
	life = parameters.attribute("life").as_float();
	attack = parameters.attribute("attack").as_float();
	defense = parameters.attribute("defense").as_float();
	magicPower = parameters.attribute("magicPower").as_float();
	dexerity = parameters.attribute("dexerity").as_float();
	charName = parameters.attribute("charName").as_string();
	maxHP = life;

	healthBar.x = position.x / 2 - 50;
	healthBar.y = position.y / 2 + 30;
	healthBar.w = 100;
	healthBar.h = 5;


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

	int lifeW = (life / maxHP) * 100;
	if (lifeW <= 0)
	{
		lifeW = 0;
	}
	healthBar.w = lifeW;
	app->render->DrawRectangle(healthBar, 0, 255, 0, 255);

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
		printf("Sable Laser attack\n");
		hasAttacked = true;
		app->battleScene->idAttack = 0;
		app->battleScene->combatState = CombatState::SELECT_ENEMY;
		break;
	case 1:
		hasAttacked = true;
		printf("Aumenta el ataque x1,5 WROAAAAR\n");
		app->battleScene->idAttack = 1;
		app->scene->allies[currentPlayerIndex]->attack *= 1.5f;
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
		printf("Pium Pium!! Pistola Laser!\n");
		app->battleScene->idAttack = 2;

		for (int i = 0; i < app->scene->enemies.Count(); ++i)
		{
			srand((unsigned)time(NULL));
			int index = rand() % 100;

			if (index <= app->scene->allies[currentPlayerIndex]->dexerity)
			{
				damage = app->scene->allies[currentPlayerIndex]->attack * 0.75f / app->scene->enemies[i]->defense * 20;
				app->scene->enemies[i]->life -= damage;
			}
		}
		
		if (app->battleScene->CheckAllPlayersAttacked()) {
			app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		}
		else
		{
			app->battleScene->currentPlayerInCombatIndex = app->battleScene->FindFirstPlayerToAttackIndex();
			app->battleScene->combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case 3:
		hasAttacked = true;
		printf("FIREBALL!!!\n");
		app->battleScene->idAttack = 3;
		damage = app->scene->allies[currentPlayerIndex]->magicPower / 2;

		for (int i = 0; i < app->scene->enemies.Count(); ++i)
		{
			app->scene->enemies[i]->life -= damage;
			printf("Has hecho %f de daño al enemigo %d\n", damage, i);
		}

		if (app->battleScene->CheckAllPlayersAttacked()) {
			app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		}
		else
		{
			app->battleScene->currentPlayerInCombatIndex = app->battleScene->FindFirstPlayerToAttackIndex();
			app->battleScene->combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case 4:
		hasAttacked = true;
		printf("DIOSA CURAME!!!\n");
		app->battleScene->idAttack = 4;

		for (int i = 0; i < app->scene->allies.Count(); ++i)
		{
			if (life < maxHP)
			{
				life += 15;
				printf("Has curado a %s\n", app->scene->allies[i]->charName.GetString());
				if (life >= maxHP)
				{
					life = maxHP;
				}
			}
		}

		if (app->battleScene->CheckAllPlayersAttacked()) {
			app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		}
		else
		{
			app->battleScene->currentPlayerInCombatIndex = app->battleScene->FindFirstPlayerToAttackIndex();
			app->battleScene->combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case 5:
		hasAttacked = true;
		printf("BOMBAZO!!!\n");
		app->battleScene->idAttack = 5;
		app->battleScene->combatState = CombatState::SELECT_ENEMY;

		break;
	}
}