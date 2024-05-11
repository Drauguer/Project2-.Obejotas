#include "Mago.h"
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
#include "SDL_mixer/include/SDL_mixer.h"

Mago::Mago() : BaseAlly()
{
	//name.Create("Mago");

	////sideWalk

	//sideWalk.PushBack({ 0,0,32,32 });
	//sideWalk.PushBack({ 32,0,32,32 });
	//sideWalk.PushBack({ 64,0,32,32 });
	//sideWalk.PushBack({ 96,0,32,32 });
	//sideWalk.PushBack({ 128,0,32,32 });
	//sideWalk.PushBack({ 160,0,32,32 });
	//sideWalk.loop = true;
	//sideWalk.speed = 0.2f;

	////frontWalk
	//frontWalk.PushBack({ 0,32,32,32 });
	//frontWalk.PushBack({ 32,32,32,32 });
	//frontWalk.PushBack({ 64,32,32,32 });
	//frontWalk.PushBack({ 96,32,32,32 });
	//frontWalk.PushBack({ 128,32,32,32 });
	//frontWalk.PushBack({ 160,32,32,32 });
	//frontWalk.loop = true;
	//frontWalk.speed = 0.2f;


	////backWalk
	//backWalk.PushBack({ 0,64,32,32 });
	//backWalk.PushBack({ 32,64,32,32 });
	//backWalk.PushBack({ 64,64,32,32 });
	//backWalk.PushBack({ 96,64,32,32 });
	//backWalk.PushBack({ 128,64,32,32 });
	//backWalk.PushBack({ 160,64,32,32 });
	//backWalk.loop = true;
	//backWalk.speed = 0.2f;

}

Mago::~Mago() {

}

bool Mago::Awake() {

	


	return true;
}

bool Mago::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	life = parameters.attribute("life").as_int();
	attack = parameters.attribute("attack").as_int();

	for (pugi::xml_node node = parameters.child("ability"); node; node = node.next_sibling("ability"))
	{
		abilityId = node.attribute("id").as_int();
		abilityName = node.attribute("name").as_string();
		char* abilityString = const_cast<char*>(abilityName);
		abilities.Add({ abilityId, abilityString });
	}

	texture = app->tex->Load(texturePath);

	int player[8] = {
		0, 0,
		12, 0,
		12, 16,
		0, 16,
	};

	pbody = app->physics->CreateChain(position.x, position.y, player, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	currentAnimation = &frontWalk;
	return true;
}

bool Mago::Update(float dt)
{
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	if (app->scene->isOnCombat)
	{
		position.x = 100;
		position.y = 200;

		//AttackScene
		if (isHighlighted) {
			//Aqui va una animacion del pesonaje resaltado


		}

		

	}
	else
	{
		
	}

	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), isFlipped);

	return true;
}

bool Mago::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Mago::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::NPC:
		LOG("Collision NPC");
		break;
	default:
		break;
	}
}