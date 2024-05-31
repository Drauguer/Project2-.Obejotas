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
#include "Inventory.h"
#include "ModuleFonts.h"

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
	case 6:
		position.x = parameters.attribute("x6").as_int() * scale;
		position.y = parameters.attribute("y6").as_int() * scale;
		break;
	case 7:
		position.x = parameters.attribute("x7").as_int() * scale;
		position.y = parameters.attribute("y7").as_int() * scale;
		break;
	case 8:
		position.x = parameters.attribute("x8").as_int() * scale;
		position.y = parameters.attribute("y8").as_int() * scale;
		break;
	case 9:
		position.x = parameters.attribute("x9").as_int() * scale;
		position.y = parameters.attribute("y9").as_int() * scale;
		break;
	case 10:
		position.x = parameters.attribute("x10").as_int() * scale;
		position.y = parameters.attribute("y10").as_int() * scale;
		break;
	case 11:
		position.x = parameters.attribute("x11").as_int() * scale;
		position.y = parameters.attribute("y11").as_int() * scale;
		break;
	case 12:
		position.x = parameters.attribute("x12").as_int() * scale;
		position.y = parameters.attribute("y12").as_int() * scale;
		break;
	case 13:
		position.x = parameters.attribute("x13").as_int() * scale;
		position.y = parameters.attribute("y13").as_int() * scale;
		break;
	case 14:
		position.x = parameters.attribute("x14").as_int() * scale;
		position.y = parameters.attribute("y14").as_int() * scale;
		break;
	case 15:
		position.x = parameters.attribute("x15").as_int() * scale;
		position.y = parameters.attribute("y15").as_int() * scale;
		break;
	case 16:
		position.x = parameters.attribute("x16").as_int() * scale;
		position.y = parameters.attribute("y16").as_int() * scale;
		break;
	
	}

	
	texturePath = parameters.attribute("texturePath").as_string();

	texture = app->tex->Load(texturePath);

	InitAnims();
	
	walkingRockFx = app->audio->LoadFx("Assets/Audio/Fx/12_Player_Movement_SFX/08Steprock02.wav");
	
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

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

	int winW = windowW;
	int winH = windowH;
	dialogueBoxPos = { (winW / 2 - 600 - app->render->camera.x) / scale, (winH / 2 + 120 - app->render->camera.y) / scale, 1200 / scale, 250 / scale };
	dialogueBoxPos2 = { (winW / 2 - 600) / scale, (winH / 2 + 120) / scale, 1200 / scale, 250 / scale };

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
		if (!isTalking)
		{
			//Movement scene
			vel = b2Vec2(0, 0);
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

				if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN || pad.x == 1 && app->selectActionCooldown==0)
				{
					

					app->inventory->isInventory = !app->inventory->isInventory;
					if (app->inventory->active)
					{
						app->inventory->Disable();
					}
					else if (app->inventory->active == false)
					{
						app->scene->isNewItem = false;
						for (int i = 0; i < app->scene->listItems.Count(); ++i)
						{
							app->scene->listItems[i]->newInInventory = false;
						}
						app->inventory->Enable();
					}
					app->audio->PlayFx(app->scene->hoverFx);
					app->selectActionCooldown = 30;
				}




			}
		}
		else
		{
			vel = b2Vec2(0, 0);
			pbody->body->SetLinearVelocity(vel);
		}
		if (app->selectActionCooldown > 0) {
			app->selectActionCooldown--;
		}
		if (showDoorLocked)
		{
			

			if (doorLockedTimer <= 120)
			{
				app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
				app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "it's not the time to bother the neighbours!");
				doorLockedTimer++;
			}
			else
			{
				doorLockedTimer = 0;
				showDoorLocked = false;
				isTalking = false;
			}
			app->audio->PlayFx(app->scene->deniedFx);
		}

		if (showPasswordWrong)
		{

			if (passwordTimer <= 120)
			{
				app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
				app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "this is not the correct password!!");
				passwordTimer++;
			}
			else
			{
				passwordTimer = 0;
				showPasswordWrong = false;
				isTalking = false;
			}

			app->audio->PlayFx(app->scene->deniedFx);
		}

		if (showPasswordCorrect)
		{
			
			if (passwordTimer2 <= 120)
			{
				app->render->DrawTexture(app->dialogueManager->chatbox, dialogueBoxPos.x, dialogueBoxPos.y);
				app->fonts->BlitText(dialogueBoxPos2.x + 30, dialogueBoxPos2.y + 15, app->dialogueManager->Font, "password correct! you can enter the castle...");
				passwordTimer2++;
			}
			else
			{
				passwordTimer2 = 0;
				showPasswordCorrect = false;
				isTalking = false;
			}
			app->audio->PlayFx(app->scene->attackUpSFX);
		}

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 10);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 10);

		if (!app->inventory->isInventory)
		{
			app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), isFlipped);
		}
		
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
	case ColliderType::TOINTERIOR2:
		if (app->scene->mapID != 3)
		{

			LoadNewMap(3, 6);

		}
		break;
	case ColliderType::TOINTERIOR3:
		if (app->scene->mapID != 4)
		{

			LoadNewMap(4, 7);

		}
		break;
	case ColliderType::TOZONE2:
		if (app->scene->mapID != 5)
		{

			LoadNewMap(5, 8);

		}
		break;
	case ColliderType::TOZONE3:
		if (app->scene->mapID != 6)
		{
			if (app->hasSolvedPasswordPuzzle)
			{
				LoadNewMap(6, 9);
			}
			else
			{
				showPasswordWrong = true;
				isTalking = true;
				
			}
			

		}
		break;
	case ColliderType::TOPUZZLE1:
		if (app->scene->mapID != 7)
		{

			LoadNewMap(7, 10);

		}
		break;
	case ColliderType::TOPUZZLE2:
		if (app->scene->mapID != 8)
		{

			LoadNewMap(8, 11);

		}
		break;
	case ColliderType::TOZ3_P1:
		if (app->scene->mapID != 6)
		{

			LoadNewMap(6, 12);

		}
		break;
	case ColliderType::TOZ3_P2:
		if (app->scene->mapID != 6)
		{

			LoadNewMap(6, 13);

		}
		break;
	case ColliderType::TO_OUT2:
		if (app->scene->mapID != 1)
		{

			LoadNewMap(1, 14);

		}
		break;
	case ColliderType::TO_OUT3:
		if (app->scene->mapID != 5)
		{

			LoadNewMap(5, 15);

		}
		break;
	case ColliderType::TO_ZONE2_OUT:
		if (app->scene->mapID != 5)
		{

			LoadNewMap(5, 16);

		}
		break;
	case ColliderType::DOOR_LOCKED:
		showDoorLocked = true;
		isTalking = true;
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
	for (int i = 0; i < app->scene->allies.Count(); ++i)
	{
		app->entityManager->entities.Add(app->scene->allies[i]);
	}
	for (int i = 0; i < app->scene->listItems.Count(); ++i)
	{
		app->entityManager->entities.Add(app->scene->listItems[i]);
	}
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
