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
#include "DialogueManager.h"
#include "ModuleFonts.h"
#include "DialogueManager.h"
#include "ModuleFonts.h"

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
		idleAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	idleAnim.speed = parameters.child("Idle").attribute("animspeed").as_float();
	idleAnim.loop = parameters.child("Idle").attribute("loop").as_bool();

	for (pugi::xml_node node = parameters.child("Attack").child("pushback"); node; node = node.next_sibling("pushback")) {
		attackAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	attackAnim.speed = parameters.child("Attack").attribute("animspeed").as_float();
	attackAnim.loop = parameters.child("Attack").attribute("loop").as_bool();

}

bool BaseAlly::Awake() 
{

	return true;
}

bool BaseAlly::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	baseLife = parameters.attribute("life").as_float();
	baseAttack = parameters.attribute("attack").as_float();
	baseDefense = parameters.attribute("defense").as_float();
	baseMagicPower = parameters.attribute("magicPower").as_float();
	baseDexerity = parameters.attribute("dexerity").as_float();
	charName = parameters.attribute("charName").as_string();
	maxHP = baseLife;

	SetStats();

	healthBar.x = (position.x - 110) / 2;
	healthBar.y = position.y / 2 + 44;
	healthBar.w = 36;
	healthBar.h = 6;

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	


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

	currentAnim = &idleAnim;

	return true;
}

bool BaseAlly::Update(float dt)
{

	int scale = app->win->GetScale();

	if (helmetItem != NULL)
	{
		SetHelmetStats();
	}
		
	if (armorItem != NULL)
	{
		SetArmorStats();
	}

	if (weaponItem != NULL)
	{
		SetWeaponStats();
	}


	
	if (attackAnim.HasFinished()) {
		currentAnim = &idleAnim;		
	}

	currentAnim->Update();

	int lifeW = (life / maxHP) * 36;
	if (lifeW <= 0)
	{
		lifeW = 0;
	}
	healthBar.w = lifeW;

	if (life > 0 && app->scene->isOnCombat) {
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x / scale)), PIXEL_TO_METERS((float32)(position.y / scale)) }, 0);
		app->render->DrawTexture(texture, position.x / scale, position.y / scale, &currentAnim->GetCurrentFrame());
		
	}

	int winW = windowW;
	int winH = windowH;

	dialogueBoxPos = { (winW / 2 - 600 - app->render->camera.x) / scale, (winH / 2 + 120 - app->render->camera.y) / scale, 1200 / scale, 250 / scale };
	dialogueBoxPos2 = { (winW / 2 - 600) / scale, (winH / 2 + 120) / scale, 1200 / scale, 250 / scale };

	

	return true;
}

bool BaseAlly::PostUpdate()
{
	

	return true;
}

bool BaseAlly::CleanUp()
{
	return true;
}

void BaseAlly::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

void BaseAlly::SetAttackAnimation() {
	attackAnim.ResetLoop();
	currentAnim = &attackAnim;
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
	//Here we check if we have to select an enemy or just make the effect 
{
	int scale = app->win->GetScale();

	switch (selectAttackIndex)
	{
	case 0:
		printf("Sable Laser attack\n");
		app->battleScene->idAttack = 0;
		app->battleScene->combatState = CombatState::SELECT_ENEMY;
		break;
	case 1:
		hasAttacked = true;
		printf("Aumenta el ataque x1,5 WROAAAAR\n");
		

		app->battleScene->idAttack = 1;
		app->scene->allies[currentPlayerIndex]->attack *= 1.5f;
		app->scene->allies[currentPlayerIndex]->SetAttackAnimation();
		if (app->battleScene->CheckAllPlayersAttacked()) {
			app->battleScene->CheckState();
			app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		}
		else
		{
			app->battleScene->currentPlayerInCombatIndex = app->battleScene->FindFirstPlayerToAttackIndex();
			app->battleScene->CheckState();
			app->battleScene->combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case 2:
		hasAttacked = true;
		printf("Pium Pium!! Visión Laser!\n");

		app->battleScene->isText = true;

		app->battleScene->idAttack = 2;
		app->scene->allies[currentPlayerIndex]->SetAttackAnimation();

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
			app->battleScene->CheckState();
			app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		}
		else
		{
			app->battleScene->currentPlayerInCombatIndex = app->battleScene->FindFirstPlayerToAttackIndex();
			app->battleScene->CheckState();
			app->battleScene->combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case 3:
		hasAttacked = true;
		printf("FIREBALL!!!\n");

		app->battleScene->isText = true;

		app->scene->allies[currentPlayerIndex]->SetAttackAnimation();

		app->battleScene->idAttack = 3;
		damage = app->scene->allies[currentPlayerIndex]->magicPower / 2;

		for (int i = 0; i < app->scene->enemies.Count(); ++i)
		{
			app->scene->enemies[i]->life -= damage;
			printf("Has hecho %f de daño al enemigo %d\n", damage, i);
		}

		if (app->battleScene->CheckAllPlayersAttacked()) {
			app->battleScene->CheckState();
			app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		}
		else
		{
			app->battleScene->currentPlayerInCombatIndex = app->battleScene->FindFirstPlayerToAttackIndex();
			app->battleScene->CheckState();
			app->battleScene->combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case 4:
		hasAttacked = true;
		printf("DIOSA CURAME!!!\n");

		app->battleScene->isText = true;

		app->scene->allies[currentPlayerIndex]->SetAttackAnimation();

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
			app->battleScene->CheckState();
			app->battleScene->combatState = CombatState::ENEMY_ATTACK;
		}
		else
		{
			app->battleScene->currentPlayerInCombatIndex = app->battleScene->FindFirstPlayerToAttackIndex();
			app->battleScene->CheckState();
			app->battleScene->combatState = CombatState::SELECT_CHARACTER;
		}
		break;
	case 5:
		printf("BOMBAZO!!!\n");
		app->battleScene->idAttack = 5;
		app->battleScene->combatState = CombatState::SELECT_ENEMY;

		break;
	case 6:
		printf("MARTILLAZO!!!\n");
		app->battleScene->idAttack = 6;
		app->battleScene->combatState = CombatState::SELECT_ENEMY;

		break;
	}
}

void BaseAlly::SetStats()
{
	life = baseLife;
	attack = baseAttack;
	defense = baseDefense;
	magicPower = baseMagicPower;
	dexerity = baseDexerity;
}

void BaseAlly::SetHelmetStats()
{
	life = baseLife + helmetItem->bonusHP;
	attack = baseAttack + helmetItem->bonusATK;
	defense = baseDefense + helmetItem->bonusDEF;
	magicPower = baseMagicPower + helmetItem->bonusMP;
	dexerity = baseDexerity + helmetItem->bonusDEX;
}

void BaseAlly::SetArmorStats()
{
	life = baseLife + armorItem->bonusHP;
	attack = baseAttack + armorItem->bonusATK;
	defense = baseDefense + armorItem->bonusDEF;
	magicPower = baseMagicPower + armorItem->bonusMP;
	dexerity = baseDexerity + armorItem->bonusDEX;
}

void BaseAlly::SetWeaponStats()
{
	life = baseLife + weaponItem->bonusHP;
	attack = baseAttack + weaponItem->bonusATK;
	defense = baseDefense + weaponItem->bonusDEF;
	magicPower = baseMagicPower + weaponItem->bonusMP;
	dexerity = baseDexerity + weaponItem->bonusDEX;
}