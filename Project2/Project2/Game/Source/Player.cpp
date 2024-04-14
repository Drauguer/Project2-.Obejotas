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
#include "Map.h"

Player::Player() : Entity(EntityType::PLAYER)
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

	
	//life = parameters.attribute("life").as_int();
	//attack = parameters.attribute("attack").as_int();

	
	
	

	return true;
}

bool Player::Start() {

	int scale = app->win->GetScale();

	switch (app->scene->playerMapID)
	{
	case 0:
		position.x = parameters.attribute("x").as_int() * scale;
		position.y = parameters.attribute("y").as_int() * scale;
		break;
	case 1:
		position.x = parameters.attribute("x1").as_int() * scale;
		position.y = parameters.attribute("y1").as_int() * scale;
		break;
	case 2:
		position.x = parameters.attribute("x2").as_int() * scale;
		position.y = parameters.attribute("y2").as_int() * scale;
		break;
	case 3:
		position.x = parameters.attribute("x3").as_int() * scale;
		position.y = parameters.attribute("y3").as_int() * scale;
		break;
	case 4:
		position.x = parameters.attribute("x4").as_int() * scale;
		position.y = parameters.attribute("y4").as_int() * scale;
		break;
	case 5:
		position.x = parameters.attribute("x5").as_int() * scale;
		position.y = parameters.attribute("y5").as_int() * scale;
		break;
	}

	
	texturePath = parameters.attribute("texturePath").as_string();

	texture = app->tex->Load(texturePath);

	InitAnims();
	
	walkingRockFx = app->audio->LoadFx("Assets/Audio/Fx/12_Player_Movement_SFX/WalkingRock.ogg");
	

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

	GamePad& pad = app->input->pads[0];


	int scale = app->win->GetScale();

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	if (app->scene->isOnCombat) 
	{
		position.x = 100;
		position.y = 150;
		b2Vec2 vel = b2Vec2(0, 0);

		//AttackScene
		if (isHighlighted) {
			//Aqui va una animacion del pesonaje resaltado
			

		}

	}
	else
	{
		//Movement scene
		b2Vec2 vel = b2Vec2(0, 0);
		if (isOnPause == false) {



			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || pad.l_x < 0) {
				vel.x += -0.2 * dt;
				isFlipped = true;
				currentAnimation = &sideWalk;
				sideWalk.Update();
				app->audio->PlayFx(walkingRockFx);

			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || pad.l_x > 0) {
				vel.x += 0.2 * dt;
				currentAnimation = &sideWalk;
				sideWalk.Update();
				isFlipped = false;
				app->audio->PlayFx(walkingRockFx);
			}
			else if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || pad.l_y < 0) {
				vel.y += -0.2 * dt;
				currentAnimation = &backWalk;
				backWalk.Update();
				app->audio->PlayFx(walkingRockFx);
			}
			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || pad.l_y > 0) {
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
		}
		

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 10);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 10);

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
		//app->audio->PlayFx(app->scene->encounterFx);
		break;
	case ColliderType::TOMAP1_LEFT:

		if (app->scene->mapID != 0)
		{
			
			LoadNewMap(0, 0);
			
		}
		
		break;
	case ColliderType::TOMAP1_RIGHT:
		if (app->scene->mapID != 0)
		{
			
			LoadNewMap(0, 1);

		}
		break;
	case ColliderType::TOMAP1_CENTER:
		if (app->scene->mapID != 0)
		{
			
			LoadNewMap(0, 5);

		}
		break;
	case ColliderType::TOMAP2_LEFT:

		if (app->scene->mapID != 1)
		{
			
			LoadNewMap(1, 2);
			
		}
		
		break;
	case ColliderType::TOMAP2_RIGHT:
		
		if (app->scene->mapID != 1)
		{
			
			LoadNewMap(1, 3);
		}
		
		
		break;
	case ColliderType::TOINTERIOR:
		if (app->scene->mapID != 2)
		{

			LoadNewMap(2, 4);

		}
		break;
	default:
		break;
	}
}

void Player::LoadNewMap(int mapID_, int playerMapID_)
{
	int scale = app->win->GetScale();

	app->scene->mapID = mapID_;
	app->scene->playerMapID = playerMapID_;
	app->map->CleanUp();
	app->entityManager->Disable();
	app->AwakeScene();
	app->physics->Start();
	app->entityManager->Enable();
	app->map->Start();
	app->render->camera.y = (-app->scene->player->position.y * scale) + app->win->screenSurface->h / 2 - 100;
}


bool Player::SaveState(pugi::xml_node node, int num)
{
	SString childName("player%d", num);


	pugi::xml_node player = node.append_child(childName.GetString());

	pugi::xml_node pos = player.append_child("Position");
	pugi::xml_attribute x = pos.append_attribute("x");
	pugi::xml_attribute y = pos.append_attribute("y");
	pugi::xml_attribute mapID = pos.append_attribute("Map_ID");
	pugi::xml_attribute playerMapID = pos.append_attribute("Player_Map_ID");

	pugi::xml_node stats = player.append_child("Stats");
	pugi::xml_attribute hp = stats.append_attribute("Life");
	pugi::xml_attribute atk = stats.append_attribute("Attack");


	x.set_value(pbody->body->GetPosition().x);
	y.set_value(pbody->body->GetPosition().y);
	mapID.set_value(app->scene->mapID);
	playerMapID.set_value(app->scene->playerMapID);

	hp.set_value(life);
	atk.set_value(attack);
	
	return true;
}



bool Player::LoadState(pugi::xml_node node, int num)
{
	SString childName("player%d", num);


	pugi::xml_node player = node.child(childName.GetString());

	
	pugi::xml_node pos = player.child("Position");
	float32 x = pos.attribute("x").as_float();
	float32 y = pos.attribute("y").as_float();
	int mapID = pos.attribute("Map_ID").as_int();
	int playerMapID = pos.attribute("Player_Map_ID").as_int();
	pugi::xml_node stats = player.child("Stats");


	
	life = stats.attribute("Life").as_int();
	attack = stats.attribute("Attack").as_int();
	LoadNewMap(mapID, playerMapID);
	app->scene->player->pbody->body->SetTransform(b2Vec2(x, y), 0);
	currentAnimation = &frontWalk;

	return true;
}
