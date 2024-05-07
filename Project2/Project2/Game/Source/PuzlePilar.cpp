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
#include "PuzlePilar.h"

#include<iostream>
#include<cstdlib>

PuzlePilar::PuzlePilar() : Entity(EntityType::PUZLEPILAR)
{
	name.Create("base pilar");
	
}

PuzlePilar::~PuzlePilar() {}

void PuzlePilar::InitAnims()
{
	// Idle
	for (pugi::xml_node node = parameters.child("idleAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		idleAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	idleAnim.speed = parameters.child("idleAnim").attribute("animspeed").as_float();
	idleAnim.loop = parameters.child("idleAnim").attribute("loop").as_bool();

	for (pugi::xml_node node = parameters.child("triggerAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		triggeredAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	triggeredAnim.speed = parameters.child("triggerAnim").attribute("animspeed").as_float();
	triggeredAnim.loop = parameters.child("triggerAnim").attribute("loop").as_bool();

}

bool PuzlePilar::Awake()
{

	return true;
}

bool PuzlePilar::Start() {

	position.x = parameters.attribute("x").as_int();
	id = parameters.attribute("id").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	charName = parameters.attribute("charName").as_string();


	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//InitAnims();
	InitAnims();

	//initilize textures
	texture = app->tex->Load(texturePath);


	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PUZLE;

	currentAnim = &idleAnim;

	return true;
}

bool PuzlePilar::Update(float dt)
{
	if (canInteract) 
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !hasPressedE) 
		{
			hasPressedE = true;
			isTriggered = true;
			app->scene->combination.Add(id);
			if (app->scene->CheckAllPilars()) {
				app->scene->CheckPilarPuzleResult();
			}

		}
	}
	if (isTriggered) {
		currentAnim = &triggeredAnim;
	}
	else
	{
		currentAnim = &idleAnim;

	}
	int scale = app->win->GetScale();
	

	currentAnim->Update();
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 10);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 10);


	app->render->DrawTexture(texture, position.x, position.y , &currentAnim->GetCurrentFrame());
	

	return true;
}

bool PuzlePilar::PostUpdate()
{

	return true;
}

bool PuzlePilar::CleanUp()
{
	return true;
}

void PuzlePilar::OnCollision(PhysBody* physA, PhysBody* physB)
{
	canInteract = false;

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		if (!isTriggered) {
			canInteract = true;
			hasPressedE = false;
		}
		break;
	}
}

bool PuzlePilar::OnCollisionStay(PhysBody* physA, PhysBody* physB)
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