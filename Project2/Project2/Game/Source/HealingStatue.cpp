#include "HealingStatue.h"
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
#include "Inventory.h"
#include "BattleScene.h"

HealingStatue::HealingStatue() : Entity(EntityType::HEALING_STATUE)
{
	name.Create("npc");
}

HealingStatue::~HealingStatue() {}

bool HealingStatue::Awake()
{


	return true;
}

bool HealingStatue::Start()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	statueOn = app->tex->Load("Assets/Textures/Healing_Statue_On.png");
	statueOff = app->tex->Load("Assets/Textures/Healing_Statue_Off.png");

	pbody = app->physics->CreateRectangle(position.x + 20, position.y + 40, 50, 80, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::HEALING_STATUE;

	range = app->physics->CreateRectangleSensor(position.x + 60, position.y + 80, 100, 100, bodyType::STATIC);
	range->listener = this;
	range->ctype = ColliderType::HEALING_STATUE;

	return true;
}

bool HealingStatue::Update(float dt)
{

	int scale = app->win->GetScale();
	GamePad& pad = app->input->pads[0];

	if (OnCollisionStay(this->range, app->scene->player->pbody) && (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.y) && canHeal)
	{
		for (int i = 0; i < app->scene->allies.Count(); ++i)
		{
			app->scene->allies[i]->life = app->scene->allies[i]->maxHP;
		}
		canHeal = false;
	}


	pbody->body->SetTransform({ PIXEL_TO_METERS((float32)((position.x + 32))), PIXEL_TO_METERS((float32)((position.y + 40))) }, 0);
	if (!app->inventory->isInventory)
	{
		if (canHeal)
		{
			app->render->DrawTexture(statueOn, (position.x), (position.y));
		}
		else if (!canHeal)
		{
			app->render->DrawTexture(statueOff, (position.x), (position.y));
		}
	}
	


	return true;
}

bool HealingStatue::CleanUp()
{
	return true;
}

void HealingStatue::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

bool HealingStatue::OnCollisionStay(PhysBody* physA, PhysBody* physB)
{
	int scale = app->win->GetScale();

	int xA, yA, xB, yB;

	physA->GetPosition(xA, yA);
	physB->GetPosition(xB, yB);

	if (xB <= xA + (physA->width) && xB >= xA && yB >= yA && yB <= yA + (physA->height))
	{
		return true;
	}

	return false;
}