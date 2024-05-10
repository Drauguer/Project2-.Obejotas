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

}

bool PuzlePassword::Awake()
{

	return true;
}

bool PuzlePassword::Start() {

	arrowTexturePath = config.attribute("texturePath").as_string();

	for (pugi::xml_node node = config.child("arrow").child("pushback"); node; node = node.next_sibling("pushback")) {
		idleArrowAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	idleArrowAnim.speed = config.child("arrow").attribute("animspeed").as_float();
	idleArrowAnim.loop = config.child("arrow").attribute("loop").as_bool();

	position.x = parameters.attribute("x").as_int();
	id = parameters.attribute("id").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	arrowTexture = app->tex->Load(arrowTexturePath);

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz 1234567890.,'=(?!)+-*/      " };
	Font = app->fonts->Load("Assets/Fonts/typography.png", lookupTable, 2);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//InitAnims();
	InitAnims();

	//initilize textures
	texture = app->tex->Load(texturePath);


	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PUZLE;

	
	SetCombinations();
	
	currentArrowAnim = &idleArrowAnim;

	return true;
}

bool PuzlePassword::Update(float dt)
{
	if (OnCollisionStay(this->pbody, app->scene->player->pbody)) 
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
			if (currentCombination[currentIndex] < 9) 
			{
				currentCombination[currentIndex]++;
				if (IsCombinationCorrect()) {
					//Aqui va el codigo de victoria 
					return false;
				}
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
			if (currentCombination[currentIndex] >0)
			{
				currentCombination[currentIndex]--;
				if (IsCombinationCorrect()) {
					//Aqui va el codigo de victoria 
					return false;
				}
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
			if (currentIndex < 1)
			{
				currentIndex = 2;
			}
			else
			{
				currentIndex--;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
			if (currentIndex > 1)
			{
				currentIndex = 0;
			}
			else
			{
				currentIndex++;
			}
		}
		DrawNumbers();
		
	}
	else
	{

	}

	
	int scale = app->win->GetScale();

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 10);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 10);

	app->render->DrawTexture(arrowTexture, (position.x + 20) / scale,
		(position.y) / scale, &currentArrowAnim->GetCurrentFrame());




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

void PuzlePassword::CheckResult()
{

}

void PuzlePassword::DrawNumbers()
{
	std::string cadena = std::to_string(currentCombination[0]);
	const char* puntero = cadena.c_str();
	app->fonts->BlitText(position.x, position.y, Font, puntero);

	cadena = std::to_string(currentCombination[1]);
	puntero = cadena.c_str();
	app->fonts->BlitText(position.x + 20, position.y, Font, puntero);

	cadena = std::to_string(currentCombination[2]);
	puntero = cadena.c_str();
	app->fonts->BlitText(position.x + 40, position.y, Font, puntero);
}

bool PuzlePassword::IsCombinationCorrect()
{

	for (int i = 0; i < currentCombination.Count(); i++)
	{
		if (currentCombination[i] != finalCombination[i]) 
		{
			return false;
		}
	}
	return true;
}

void PuzlePassword::SetCombinations() 
{
	currentCombination.Add(0);
	currentCombination.Add(0);
	currentCombination.Add(0);

	finalCombination.Add(5);
	finalCombination.Add(8);
	finalCombination.Add(4);
}