#include "Player.h"
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
#include "Window.h"

Player::Player() : BaseAlly()
{
	name.Create("Player");

	

}

Player::~Player() {

}

void Player::InitAnims()
{
	// frontWalk
	for (pugi::xml_node node = parameters.child("frontWalk").child("pushback"); node; node = node.next_sibling("pushback")) {
		frontWalk.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	frontWalk.speed = parameters.child("frontWalk").attribute("animspeed").as_float();
	frontWalk.loop = parameters.child("frontWalk").attribute("loop").as_bool();

	// backWalk
	for (pugi::xml_node node = parameters.child("backWalk").child("pushback"); node; node = node.next_sibling("pushback")) {
		backWalk.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	backWalk.speed = parameters.child("backWalk").attribute("animspeed").as_float();
	backWalk.loop = parameters.child("backWalk").attribute("loop").as_bool();

	// sideWalk
	for (pugi::xml_node node = parameters.child("sideWalk").child("pushback"); node; node = node.next_sibling("pushback")) {
		sideWalk.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	sideWalk.speed = parameters.child("sideWalk").attribute("animspeed").as_float();
	sideWalk.loop = parameters.child("sideWalk").attribute("loop").as_bool();

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	//life = parameters.attribute("life").as_int();
	//attack = parameters.attribute("attack").as_int();

	for (pugi::xml_node node = parameters.child("ability"); node; node = node.next_sibling("ability"))
	{
		abilityId = node.attribute("id").as_int();
		abilityName = node.attribute("name").as_string();
		char* abilityString = const_cast<char*>(abilityName);
		abilities.Add({ abilityId, abilityString });
	}
	
	InitAnims();

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());
	
	walkingRockFx = app->audio->LoadFx("Assets/Audio/Fx/12_Player_Movement_SFX/08Steprock02.wav");
	

	int player[8] = {
		0, 0,
		12, 0,
		12, 16,
		0, 16,
	};

	pbody = app->physics->CreateChain(position.x / 2, position.y / 2, player, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	currentAnimation = &frontWalk;
	return true;
}

bool Player::Update(float dt)
{

	int scale = app->win->GetScale();

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	if (app->scene->isOnCombat) 
	{
		position.x = 100;
		position.y = 150;

		//AttackScene
		if (isHighlighted) {
			//Aqui va una animacion del pesonaje resaltado
			

		}

	}
	else
	{
		//Movement scene
	
		b2Vec2 vel = b2Vec2(0, 0);

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel.x += -0.2 * dt;
			isFlipped = true;
			currentAnimation = &sideWalk;
			sideWalk.Update();
			app->audio->PlayFx(walkingRockFx);
			
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel.x += 0.2 * dt;
			currentAnimation = &sideWalk;
			sideWalk.Update();	
			isFlipped = false;
			app->audio->PlayFx(walkingRockFx);
		}
		else if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel.y += -0.2 * dt;
			currentAnimation = &backWalk;
			backWalk.Update();
			app->audio->PlayFx(walkingRockFx);
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel.y += 0.2 * dt;
			currentAnimation = &frontWalk;
			frontWalk.Update();
			app->audio->PlayFx(walkingRockFx);
		}
		else {
			sideWalk.Reset();
			frontWalk.Reset();
			backWalk.Reset();
		}

		pbody->body->SetLinearVelocity(vel);

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

		//pbody->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x / scale)), PIXEL_TO_METERS((float32)(position.y / scale)) }, 0);

		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), isFlipped);
	}

	

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
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

// Mira que ataque tiene que hacer el personaje
void Player::CheckAttack(int selectAttackIndex, int currentPlayerIndex)
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
	}
}

