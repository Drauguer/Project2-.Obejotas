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
	

	Mago = app->tex->Load("Assets/Textures/MagoPortrait.png");
	Enano = app->tex->Load("Assets/Textures/EnanoPortrait.png");
	Eldrin = app->tex->Load("Assets/Textures/EldrinPortrait.png");
	Bardo = app->tex->Load("Assets/Textures/BardoPortrait.png");

	arrowTexture = app->tex->Load(arrowTexturePath);

	return true;
}

bool Inventory::Update(float dt)
{
	int scale = app->win->GetScale();

	for (int i = 0; i < app->scene->allies.Count(); ++i)
	{
		int t = 0;

		app->render->DrawRectangle({ 35 + i * 150 - app->render->camera.x / scale, 25 - app->render->camera.y / scale, 140, 300 }, 0, 0, 255, 255);
		
		if (strcmp(app->scene->allies[i]->charName.GetString(), "Gale el Mago") == 0)
		{
			

			for (int h = 0; h < 3; ++h)
			{
				app->render->DrawRectangle({ 135 + i * 150 - app->render->camera.x / scale, 30 + 45 * h - app->render->camera.y / scale, 35, 35 }, 0, 255, 0, 255);
			}

			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					app->render->DrawRectangle({ 42 + i * 150 + k * 45 - app->render->camera.x / scale, 180 + j * 45 - app->render->camera.y / scale, 35, 35 }, 0, 255, 0, 255);
					
					if (t < app->scene->allies[i]->inventoryChar.Count())
					{
						if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
						{
							switch (app->scene->allies[i]->inventoryChar[t]->itemType)
							{
							case ItemType::HELMET:
								app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 - app->render->camera.y / scale;
								break;
							case ItemType::ARMOR:
								app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 + 45 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45 - app->render->camera.y / scale;
								break;
							case ItemType::WEAPON:
								app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 + 90 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90 - app->render->camera.y / scale;
								break;
							}

						}
						else {
							app->render->DrawRectangle({ 42 + i * 150 + k * 50 - app->render->camera.x / scale, 180 + j * 50 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 42 + i * 150 + k * 50 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 180 + j * 50 - app->render->camera.y / scale;
						}
						t++;
					}
					
				}

			}

			app->render->DrawTexture(Mago, 75 + i * 150 - app->render->camera.x / scale, 45 - app->render->camera.y / scale);

			t = 0;
		} 
		else if (strcmp(app->scene->allies[i]->charName.GetString(), "David el Enano") == 0)
		{
			

			for (int h = 0; h < 3; ++h)
			{
				app->render->DrawRectangle({ 135 + i * 150 - app->render->camera.x / scale, 30 + 45 * h - app->render->camera.y / scale, 35, 35 }, 0, 255, 0, 255);
			}

			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					app->render->DrawRectangle({ 42 + i * 150 + k * 45 - app->render->camera.x / scale, 180 + j * 45 - app->render->camera.y / scale, 35, 35 }, 0, 255, 0, 255);

					if (t < app->scene->allies[i]->inventoryChar.Count())
					{
						if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
						{
							switch (app->scene->allies[i]->inventoryChar[t]->itemType)
							{
							case ItemType::HELMET:
								app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 - app->render->camera.y / scale;
								break;
							case ItemType::ARMOR:
								app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 + 45 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45 - app->render->camera.y / scale;
								break;
							case ItemType::WEAPON:
								app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 + 90 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90 - app->render->camera.y / scale;
								break;
							}
							
						}
						else {
							app->render->DrawRectangle({ 42 + i * 150 + k * 50 - app->render->camera.x / scale, 180 + j * 50 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 42 + i * 150 + k * 50 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 180 + j * 50 - app->render->camera.y / scale;
						}
						
						t++;
					}
				}

			}

			app->render->DrawTexture(Enano, 75 + i * 150 - app->render->camera.x / scale, 45 - app->render->camera.y / scale);

			t = 0;
		}
		else if (strcmp(app->scene->allies[i]->charName.GetString(), "Eldrin") == 0)
		{


		for (int h = 0; h < 3; ++h)
		{
			app->render->DrawRectangle({ 135 + i * 150 - app->render->camera.x / scale, 30 + 45 * h - app->render->camera.y / scale, 35, 35 }, 0, 255, 0, 255);
		}

		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				app->render->DrawRectangle({ 42 + i * 150 + k * 45 - app->render->camera.x / scale, 180 + j * 45 - app->render->camera.y / scale, 35, 35 }, 0, 255, 0, 255);

				if (t < app->scene->allies[i]->inventoryChar.Count())
				{
					if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
					{
						switch (app->scene->allies[i]->inventoryChar[t]->itemType)
						{
						case ItemType::HELMET:
							app->render->DrawRectangle({ 140 + i * 150, 30 - app->render->camera.x / scale, 25 - app->render->camera.y / scale, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 - app->render->camera.y / scale;
							break;
						case ItemType::ARMOR:
							app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 + 45 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45 - app->render->camera.y / scale;
							break;
						case ItemType::WEAPON:
							app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 + 90 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90 - app->render->camera.y / scale;
							break;
						}

					}
					else {
						//app->render->DrawRectangle({ 42 + i * 150 + k * 50 - app->render->camera.x / scale, 180 + j * 50 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
						app->scene->allies[i]->inventoryChar[t]->position.x = 42 + i * 150 + k * 50 - app->render->camera.x / scale;
						app->scene->allies[i]->inventoryChar[t]->position.y = 180 + j * 50 - app->render->camera.y / scale;
					}

					t++;
				}
			}

		}

		app->render->DrawTexture(Eldrin, 75 + i * 150 - app->render->camera.x / scale, 45 - app->render->camera.y / scale);

		t = 0;
		}
		else if (strcmp(app->scene->allies[i]->charName.GetString(), "Urgo el Bardo") == 0)
		{


		for (int h = 0; h < 3; ++h)
		{
			app->render->DrawRectangle({ 135 + i * 150 - app->render->camera.x / scale, 30 + 45 * h - app->render->camera.y / scale, 35, 35 }, 0, 255, 0, 255);
		}

		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				app->render->DrawRectangle({ 42 + i * 150 + k * 45 - app->render->camera.x / scale, 180 + j * 45 - app->render->camera.y / scale, 35, 35 }, 0, 255, 0, 255);

				if (t < app->scene->allies[i]->inventoryChar.Count())
				{
					if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
					{
						switch (app->scene->allies[i]->inventoryChar[t]->itemType)
						{
						case ItemType::HELMET:
							app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 - app->render->camera.y / scale;
							break;
						case ItemType::ARMOR:
							app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 + 45 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45 - app->render->camera.y / scale;
							break;
						case ItemType::WEAPON:
							app->render->DrawRectangle({ 140 + i * 150 - app->render->camera.x / scale, 30 + 90 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90 - app->render->camera.y / scale;
							break;
						}

					}
					else {
						app->render->DrawRectangle({ 42 + i * 150 + k * 50 - app->render->camera.x / scale, 180 + j * 50 - app->render->camera.y / scale, 25, 25 }, 0, 255, 255, 255);
						app->scene->allies[i]->inventoryChar[t]->position.x = 42 + i * 150 + k * 50 - app->render->camera.x / scale;
						app->scene->allies[i]->inventoryChar[t]->position.y = 180 + j * 50 - app->render->camera.y / scale;
					}

					t++;
				}
			}

		}

		app->render->DrawTexture(Bardo, 75 + i * 150 - app->render->camera.x / scale, 45 - app->render->camera.y / scale);

		t = 0;
		}

		
	}

	

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && itemSelectedIndex + 1 < app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex + 1]->isEquipped == false)
	{
		app->audio->PlayFx(app->scene->hoverFx);
		itemSelectedIndex += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && itemSelectedIndex - 1 >= 0 && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex - 1]->isEquipped == false)
	{
		app->audio->PlayFx(app->scene->hoverFx);
		itemSelectedIndex -= 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && itemSelectedIndex + 3 < app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex + 3]->isEquipped == false)
	{
		app->audio->PlayFx(app->scene->hoverFx);
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

		app->audio->PlayFx(app->scene->hoverFx);
		
	}
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && itemSelectedIndex - 3 >= 0 && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex - 3]->isEquipped == false)
	{
		app->audio->PlayFx(app->scene->hoverFx);
		itemSelectedIndex -= 3;
	}
	else if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		app->audio->PlayFx(app->scene->hoverFx);
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

		app->audio->PlayFx(app->scene->clickFx);

		if (itemSelectedIndex == app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() - 1)
		{
			itemSelectedIndex -= 1;
		}
		else {
			itemSelectedIndex += 1;
		}

		if (app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() <= 1)
		{
			inventorySelectedIndex = 0;
			itemSelectedIndex = 0;
		}
	}

	
	
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{

			if (inventorySelectedIndex + 1 >= app->scene->allies.Count())
			{
				inventorySelectedIndex = 0;
				itemSelectedIndex = 0;
			}
			else 
			{
				if (inventorySelectedIndex + 1 < app->scene->allies.Count())
				{
					inventorySelectedIndex += 1;
					itemSelectedIndex = 0;
				}
			}
			
			app->audio->PlayFx(app->scene->declineFx);

		}
	

	currentArrowAnim = &idleArrowAnim;
	currentArrowAnim->Update();

	if (app->scene->allies[inventorySelectedIndex]->inventoryChar.start != NULL)
	{
		app->render->DrawTexture(arrowTexture, app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex]->position.x - 10, app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex]->position.y - 15, &currentArrowAnim->GetCurrentFrame());
	}
	
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