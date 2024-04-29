#include "Inventory.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Window.h"
#include "ModuleFonts.h"
#include "BattleScene.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

Inventory::Inventory(bool startEnabled) : Module(startEnabled)
{
	name.Create("inventory");
}

// Destructor
Inventory::~Inventory()
{

}

bool Inventory::Awake(pugi::xml_node config)
{

	arrowTexturePath = config.attribute("texturePath").as_string();

	for (pugi::xml_node node = config.child("arrow").child("pushback"); node; node = node.next_sibling("pushback")) {
		idleArrowAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	idleArrowAnim.speed = config.child("arrow").attribute("animspeed").as_float();
	idleArrowAnim.loop = config.child("arrow").attribute("loop").as_bool();

	return true;
}

bool Inventory::Start()
{
	app->scene->LoadAllies();

	Mago = app->tex->Load("Assets/Textures/MagoPortrait.png");
	Enano = app->tex->Load("Assets/Textures/EnanoPortrait.png");

	arrowTexture = app->tex->Load(arrowTexturePath);

	return true;
}

bool Inventory::Update(float dt)
{

	for (int i = 0; i < app->scene->allies.Count(); ++i)
	{
		int t = 0;

		app->render->DrawRectangle({ 35 + i * 150, 25, 140, 300 }, 0, 0, 255, 255);
		
		if (strcmp(app->scene->allies[i]->charName.GetString(), "Gale el Mago") == 0)
		{
			app->render->DrawTexture(Mago, 75 + i * 150, 45);

			for (int h = 0; h < 3; ++h)
			{
				app->render->DrawRectangle({ 135 + i * 150, 30 + 45 * h, 35, 35 }, 0, 255, 0, 255);
			}

			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					app->render->DrawRectangle({ 42 + i * 150 + k * 45, 180 + j * 45, 35, 35 }, 0, 255, 0, 255);

					
					if (t < app->scene->allies[i]->inventoryChar.Count())
					{
						if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
						{
							switch (app->scene->allies[i]->inventoryChar[t]->itemType)
							{
							case ItemType::HELMET:
								app->render->DrawRectangle({ 140 + i * 150, 30, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30;
								break;
							case ItemType::ARMOR:
								app->render->DrawRectangle({ 140 + i * 150, 30 + 45, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45;
								break;
							case ItemType::WEAPON:
								app->render->DrawRectangle({ 140 + i * 150, 30 + 90, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90;
								break;
							}

						}
						else {
							app->render->DrawRectangle({ 42 + i * 150 + k * 50, 180 + j * 50, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 42 + i * 150 + k * 50;
							app->scene->allies[i]->inventoryChar[t]->position.y = 180 + j * 50;
						}
						t++;
					}
					
				}

			}

			

			t = 0;
		} 
		else if (strcmp(app->scene->allies[i]->charName.GetString(), "David el Enano") == 0)
		{
			app->render->DrawTexture(Enano, 75 + i * 150, 45);

			for (int h = 0; h < 3; ++h)
			{
				app->render->DrawRectangle({ 135 + i * 150, 30 + 45 * h, 35, 35 }, 0, 255, 0, 255);
			}

			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					app->render->DrawRectangle({ 42 + i * 150 + k * 45, 180 + j * 45, 35, 35 }, 0, 255, 0, 255);

					if (t < app->scene->allies[i]->inventoryChar.Count())
					{
						if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
						{
							switch (app->scene->allies[i]->inventoryChar[t]->itemType)
							{
							case ItemType::HELMET:
								app->render->DrawRectangle({ 140 + i * 150, 30, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30;
								break;
							case ItemType::ARMOR:
								app->render->DrawRectangle({ 140 + i * 150, 30 + 45, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45;
								break;
							case ItemType::WEAPON:
								app->render->DrawRectangle({ 140 + i * 150, 30 + 90, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90;
								break;
							}
							
						}
						else {
							app->render->DrawRectangle({ 42 + i * 150 + k * 50, 180 + j * 50, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 42 + i * 150 + k * 50;
							app->scene->allies[i]->inventoryChar[t]->position.y = 180 + j * 50;
						}
						
						t++;
					}
				}

			}

			

			t = 0;
		}

		
	}

	

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && itemSelectedIndex + 1 < app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex + 1]->isEquipped == false)
	{
		itemSelectedIndex += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && itemSelectedIndex - 1 >= 0 && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex - 1]->isEquipped == false)
	{
		itemSelectedIndex -= 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && itemSelectedIndex + 3 < app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex + 3]->isEquipped == false)
	{
		itemSelectedIndex += 3;
	}
	else if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		int count = app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() - 1;
		if (app->scene->allies[inventorySelectedIndex]->inventoryChar[count]->isEquipped)
		{
			itemSelectedIndex = app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() - 2;
		}
		else {
			itemSelectedIndex = app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() - 1;
		}
		
	}
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && itemSelectedIndex - 3 >= 0 && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex - 3]->isEquipped == false)
	{
		itemSelectedIndex -= 3;
	}
	else if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (app->scene->allies[inventorySelectedIndex]->inventoryChar[0]->isEquipped)
		{
			itemSelectedIndex = 1;
		}
		else {
			itemSelectedIndex = 0;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{

		

		switch (app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex]->itemType)
		{
		case ItemType::HELMET:
			if (app->scene->allies[inventorySelectedIndex]->isHelmetEquipped)
			{
				app->scene->allies[inventorySelectedIndex]->helmetItem->isEquipped = false;
				app->scene->allies[inventorySelectedIndex]->helmetItem = app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex];
				app->scene->allies[inventorySelectedIndex]->helmetItem->isEquipped = true;
			}
			else {
				app->scene->allies[inventorySelectedIndex]->helmetItem = app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex];
				app->scene->allies[inventorySelectedIndex]->helmetItem->isEquipped = true;
			}
			app->scene->allies[inventorySelectedIndex]->isHelmetEquipped = true;
			break;
		case ItemType::ARMOR:
			if (app->scene->allies[inventorySelectedIndex]->isArmorEquipped)
			{
				app->scene->allies[inventorySelectedIndex]->armorItem->isEquipped = false;
				app->scene->allies[inventorySelectedIndex]->armorItem = app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex];
				app->scene->allies[inventorySelectedIndex]->armorItem->isEquipped = true;
			}
			else {
				app->scene->allies[inventorySelectedIndex]->armorItem = app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex];
				app->scene->allies[inventorySelectedIndex]->armorItem->isEquipped = true;
			}
			app->scene->allies[inventorySelectedIndex]->isArmorEquipped = true;
			break;
		case ItemType::WEAPON:
			if (app->scene->allies[inventorySelectedIndex]->isWeaponEquipped)
			{
				app->scene->allies[inventorySelectedIndex]->weaponItem->isEquipped = false;
				app->scene->allies[inventorySelectedIndex]->weaponItem = app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex];
				app->scene->allies[inventorySelectedIndex]->weaponItem->isEquipped = true;
			}
			else {
				app->scene->allies[inventorySelectedIndex]->weaponItem = app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex];
				app->scene->allies[inventorySelectedIndex]->weaponItem->isEquipped = true;
			}
			app->scene->allies[inventorySelectedIndex]->isWeaponEquipped = true;
			break;
		}

		if (itemSelectedIndex == app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() - 1)
		{
			itemSelectedIndex -= 1;
		}
		else {
			itemSelectedIndex += 1;
		}
	}

	
	
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{

			if (inventorySelectedIndex + 1 >= app->scene->allies.Count())
			{
				inventorySelectedIndex = 0;
				itemSelectedIndex = 0;
			}
			else if (app->scene->allies[inventorySelectedIndex + 1]->inventoryChar.start != NULL)
			{
				if (inventorySelectedIndex + 1 < app->scene->allies.Count())
				{
					inventorySelectedIndex += 1;
					itemSelectedIndex = 0;
				}
			}
			else if (app->scene->allies[inventorySelectedIndex + 2]->inventoryChar.start != NULL)
			{
				if (inventorySelectedIndex + 2 < app->scene->allies.Count())
				{
					inventorySelectedIndex += 2;
					itemSelectedIndex = 0;
				}
			}
			else if (app->scene->allies[inventorySelectedIndex + 3]->inventoryChar.start != NULL)
			{
				if (inventorySelectedIndex + 3 < app->scene->allies.Count())
				{
					inventorySelectedIndex += 3;
					itemSelectedIndex = 0;
				}
			}
				
			
		}
	

	currentArrowAnim = &idleArrowAnim;
	currentArrowAnim->Update();

	app->render->DrawTexture(arrowTexture, app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex]->position.x - 10, app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex]->position.y - 15, &currentArrowAnim->GetCurrentFrame());

	return true;
}

bool Inventory::CleanUp()
{

	return true;
}

void Inventory::Enable()
{
	if (!active)
	{
		active = true;

		Start();
	}
}

void Inventory::Disable()
{
	if (active)
	{
		active = false;
		CleanUp();
	}
}