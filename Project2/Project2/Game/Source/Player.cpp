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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	int player[8] = {
		0, 0,
		12, 0,
		12, 16,
		0, 16,
	};

	pbody = app->physics->CreateChain(position.x, position.y, player, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	return true;
}

bool Player::Update(float dt)
{
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	if (app->scene->isOnCombat) 
	{
		//AttackScene
		if (isHighlighted) {
			//Aqui va una animacion del pesonaje resaltado
			position.x = 100;
			position.y = 100;

		}

	}
	else
	{
		//Movement scene
		b2Vec2 vel = b2Vec2(0, 0);

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel.x += -0.2 * dt;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel.x += 0.2 * dt;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel.y += -0.2 * dt;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel.y += 0.2 * dt;
		}

		pbody->body->SetLinearVelocity(vel);

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 5);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 7);

		//pbody->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x)), PIXEL_TO_METERS((float32)(position.y)) }, 0);
	}

	app->render->DrawTexture(texture,position.x,position.y);

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