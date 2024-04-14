#include "BaseEnemy.h"
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
#include "List.h"

BaseEnemy::BaseEnemy() : Entity(EntityType::ENEMY)
{
	name.Create("base enemy");
}

BaseEnemy::~BaseEnemy() {}

void BaseEnemy::InitDialogues()
{
	// Load all the dialogues
	for (pugi::xml_node node = parameters.child("dialogue"); node; node = node.next_sibling("dialogue")) {
		dialogueChar = node.attribute("text").as_string();
		dialogueString = dialogueChar;
		dialoguesNPC.Add(dialogueString);
	}

}

void BaseEnemy::InitAnims()
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

bool BaseEnemy::Awake() {

	return true;
}

bool BaseEnemy::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	life = parameters.attribute("life").as_float();
	attack = parameters.attribute("attack").as_float();
	defense = parameters.attribute("defense").as_float();
	magicPower = parameters.attribute("magicPower").as_float();
	dexerity = parameters.attribute("dexerity").as_float();
	maxHP = life;

	healthBar.x = (position.x + 138) / 2;
	healthBar.y = position.y / 2 + 44;
	healthBar.w = 36;
	healthBar.h = 6;

	//load Audio


	//Init Animations;
	InitAnims();

	//Load Abilities 
	for (pugi::xml_node node = parameters.child("ability"); node; node = node.next_sibling("ability"))
	{
		abilityId = node.attribute("id").as_int();
		abilityName = node.attribute("name").as_string();
		char* abilityString = const_cast<char*>(abilityName);
		abilities.Add({ abilityId, abilityString });
		numAttacks++;
	}


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

bool BaseEnemy::Update(float dt)
{

	int scale = app->win->GetScale();

	if (attackAnim.HasFinished()) {
		currentAnim = &idleAnim;
	}

 	currentAnim->Update();

	float lifeW = (life / maxHP) * 36;
	if (lifeW <= 0)
	{
		lifeW = 0;
	}
	healthBar.w = lifeW;


	if (life > 0 && app->scene->isOnCombat) {
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x / scale)), PIXEL_TO_METERS((float32)(position.y / scale)) }, 0);
		app->render->DrawTexture(texture, position.x / scale, position.y / scale, &currentAnim->GetCurrentFrame(), true);
		


	}
	

	


	return true;
}

bool BaseEnemy::CleanUp()
{
	return true;
}


void BaseEnemy::SetAttackAnimation() {
	attackAnim.ResetLoop();
	currentAnim = &attackAnim;
}

void BaseEnemy::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

bool BaseEnemy::OnCollisionStay(PhysBody* physA, PhysBody* physB)
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