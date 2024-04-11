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

bool BaseEnemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	life = parameters.attribute("life").as_int();
	attack = parameters.attribute("attack").as_int();

	//InitDialogues();

	return true;
}

bool BaseEnemy::Start() {

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

bool BaseEnemy::Update(float dt)
{

	int scale = app->win->GetScale();

	if (life > 0) {
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x / scale)), PIXEL_TO_METERS((float32)(position.y / scale)) }, 0);
		app->render->DrawTexture(texture, position.x / scale, position.y / scale);
	}
	

	return true;
}

bool BaseEnemy::CleanUp()
{
	return true;
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