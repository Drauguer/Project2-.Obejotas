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
#include "PuzlePassword.h"

#include<iostream>
#include<cstdlib>
#include <cmath> 
#include <time.h>

PuzlePassword::PuzlePassword() : Entity(EntityType::PUZLE_PASSWORD)
{
	name.Create("password");
	
}

PuzlePassword::~PuzlePassword() {}

void PuzlePassword::InitAnims()
{
	// Idle
	for (pugi::xml_node node = parameters.child("upAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		upArrow.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	upArrow.speed = parameters.child("upAnim").attribute("animspeed").as_float();
	upArrow.loop = parameters.child("upAnim").attribute("loop").as_bool();
	
	for (pugi::xml_node node = parameters.child("downAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		downArrow.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	downArrow.speed = parameters.child("downAnim").attribute("animspeed").as_float();
	downArrow.loop = parameters.child("downAnim").attribute("loop").as_bool();
	
	for (pugi::xml_node node = parameters.child("leftAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		leftArrow.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	leftArrow.speed = parameters.child("leftAnim").attribute("animspeed").as_float();
	leftArrow.loop = parameters.child("leftAnim").attribute("loop").as_bool();

	for (pugi::xml_node node = parameters.child("rightAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		rightArrow.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	rightArrow.speed = parameters.child("rightAnim").attribute("animspeed").as_float();
	rightArrow.loop = parameters.child("rightAnim").attribute("loop").as_bool();

}

bool PuzlePassword::Awake()
{

	return true;
}

bool PuzlePassword::Start() {

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


	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PUZLE;

	return true;
}

bool PuzlePassword::Update(float dt)
{
	if (OnCollisionStay(this->pbody, app->scene->player->pbody))
	{
		if (!hasCreatedPassword) 
		{
			GeneratePassword();
		}
		if (solutionIds.Count() > 0) 
		{
			if (solutionIds.Count() > 0)
			{
				for (int i = 0; i < solutionIds.Count(); i++)
				{
					switch (solutionIds[i])
					{
					case 1:
						currentAnim = &leftArrow;
						break;
					case 2:
						currentAnim = &rightArrow;
						break;
					case 3:
						currentAnim = &upArrow;
						break;
					case 4:
						currentAnim = &downArrow;
						break;
					default:
						break;
					}
					//El 200 se cambia mas adelante cuando tenga un sprite mas pequeño 
					app->render->DrawTexture(texture, position.x + i * 200, position.y, &currentAnim->GetCurrentFrame());
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
				if (solutionIds[0] == 3) {
					solutionIds.RemoveAt(0);
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
				if (solutionIds[0] == 4) {
					solutionIds.RemoveAt(0);
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
				if (solutionIds[0] == 1) {
					solutionIds.RemoveAt(0);
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
				if (solutionIds[0] == 2) {
					solutionIds.RemoveAt(0);
				}
			}
		}
		
		


	}
	

	int scale = app->win->GetScale();
	
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 10);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 10);


	

	return true;
}

bool PuzlePassword::PostUpdate()
{

	return true;
}

bool PuzlePassword::CleanUp()
{
	return true;
}

void PuzlePassword::OnCollision(PhysBody* physA, PhysBody* physB)
{
	
}

bool PuzlePassword::OnCollisionStay(PhysBody* physA, PhysBody* physB)
{
	int scale = app->win->GetScale();

	int xA, yA, xB, yB;

	physA->GetPosition(xA, yA);
	physB->GetPosition(xB, yB);

	// Calculamos la distancia entre los objetos en el eje x e y
	int distanceX = fabs(xA - xB);
	int distanceY = fabs(yA - yB);

	// Si la distancia entre los objetos es menor o igual a 20 píxeles en ambas dimensiones
	if (distanceX <= 30 && distanceY <= 30)
	{
		return true;
	}

	return false;
}

void PuzlePassword::GeneratePassword() {
	//La forma random esta bug asi que demomento es hardcoded 
	//El 7 es la cantidad de flechas para hacer 
	//for (int i = 0; i < 4; i++)
	//{
	//	srand(clock());

	//	// Generar un número aleatorio en el rango [min, max]
	//	int min = 1;
	//	int max = 4;
	//	int randomNumber = rand() % (max - min + 1) + min;
	//	solutionIds.Add(randomNumber);
	//}

	solutionIds.Add(1);
	solutionIds.Add(4);
	solutionIds.Add(4);
	solutionIds.Add(2);
	solutionIds.Add(3);
	solutionIds.Add(2);
	solutionIds.Add(4);
	solutionIds.Add(1);
	hasCreatedPassword = true;
	
}

void PuzlePassword::CheckResult() 
{
	
}