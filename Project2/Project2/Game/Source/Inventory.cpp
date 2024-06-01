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
#include "DialogueManager.h"

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
	
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz 1234567890.,'=(?!)+-*/      " };
	Font = app->fonts->Load("Assets/Fonts/typography.png", lookupTable, 2);

	Mago = app->tex->Load("Assets/Textures/MagoPortrait.png");
	Enano = app->tex->Load("Assets/Textures/EnanoPortrait.png");
	Eldrin = app->tex->Load("Assets/Textures/EldrinPortrait.png");
	Bardo = app->tex->Load("Assets/Textures/BardoPortrait.png");
	itemText = app->tex->Load("Assets/Textures/Item_Text.png");
	itemBox = app->tex->Load("Assets/Textures/OliverAssets/Inventory/Item_Box.png");
	charBox = app->tex->Load("Assets/Textures/OliverAssets/Inventory/Char_Box.png");
	atkIcon = app->tex->Load("Assets/Textures/OliverAssets/Inventory/Atk_icon.png");
	defIcon = app->tex->Load("Assets/Textures/OliverAssets/Inventory/Def_icon.png");
	mpIcon = app->tex->Load("Assets/Textures/OliverAssets/Inventory/MP_icon.png");
	dexIcon = app->tex->Load("Assets/Textures/OliverAssets/Inventory/Dex_icon.png");
	healthIcon = app->tex->Load("Assets/Textures/LifeEnemy.png");

	arrowTexture = app->tex->Load(arrowTexturePath);

	return true;
}

bool Inventory::Update(float dt)
{
	int scale = app->win->GetScale();

	GamePad& pad = app->input->pads[0];


	for (int i = 0; i < app->scene->allies.Count(); ++i)
	{
		int t = 0;

		app->render->DrawTexture(charBox, 35 + i * 150 - app->render->camera.x / scale, 25 - app->render->camera.y / scale);
		
		if (strcmp(app->scene->allies[i]->charName.GetString(), "Gale el Mago") == 0)
		{
			
			app->render->DrawTexture(atkIcon, 50 + i * 150 - app->render->camera.x / scale, 25 - app->render->camera.y / scale);
			int atkNum = app->scene->allies[i]->attack;
			std::string atk = std::to_string(atkNum);
			app->fonts->BlitText(205, 50, Font, atk.c_str());
			app->render->DrawTexture(defIcon, 50 + i * 150 - app->render->camera.x / scale, 60 - app->render->camera.y / scale);
			int defNum = app->scene->allies[i]->defense;
			std::string def = std::to_string(defNum);
			app->fonts->BlitText(205, 85, Font, def.c_str());
			app->render->DrawTexture(mpIcon, 50 + i * 150 - app->render->camera.x / scale, 95 - app->render->camera.y / scale);
			int mpNum = app->scene->allies[i]->magicPower;
			std::string mp = std::to_string(mpNum);
			app->fonts->BlitText(205, 120, Font, mp.c_str());
			app->render->DrawTexture(dexIcon, 50 + i * 150 - app->render->camera.x / scale, 130 - app->render->camera.y / scale);
			int dexNum = app->scene->allies[i]->dexerity;
			std::string dex = std::to_string(dexNum);
			app->fonts->BlitText(205, 155, Font, dex.c_str());

			for (int h = 0; h < 3; ++h)
			{

				app->render->DrawTexture(itemBox, 135 + i * 150 - app->render->camera.x / scale, 30 + 45 * h - app->render->camera.y / scale);
				
			}

			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					app->render->DrawTexture(itemBox, 39 + i * 150 + k * 45 - app->render->camera.x / scale, 180 + j * 45 - app->render->camera.y / scale);
					
					if (t < app->scene->allies[i]->inventoryChar.Count())
					{
						if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
						{
							switch (app->scene->allies[i]->inventoryChar[t]->itemType)
							{
							case ItemType::HELMET:
								
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 - app->render->camera.y / scale;
								break;
							case ItemType::ARMOR:
								
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45 - app->render->camera.y / scale;
								break;
							case ItemType::WEAPON:
								
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90 - app->render->camera.y / scale;
								break;
							}

						}
						else {
							
							app->scene->allies[i]->inventoryChar[t]->position.x = 47 + i * 150 + k * 45 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 190 + j * 43 - app->render->camera.y / scale;
						}
						t++;
					}
					
				}

			}

			SDL_Rect healthBar;
			healthBar.x = 95 + i * 150 - app->render->camera.x / scale;
			healthBar.y = 120 - app->render->camera.y / scale;
			healthBar.w = 36;
			healthBar.h = 6;

			app->render->DrawTexture(Mago, 75 + i * 150 - app->render->camera.x / scale, 45 - app->render->camera.y / scale);

			int lifeW = (app->scene->allies[i]->life / app->scene->allies[i]->maxHP) * 36;
			if (lifeW <= 0)
			{
				lifeW = 0;
			}
			healthBar.w = lifeW;

			app->render->DrawRectangle(healthBar, 0, 255, 0, 255);

			app->render->DrawTexture(healthIcon, 75 + i * 150 - app->render->camera.x / scale, 75 - app->render->camera.y / scale);

			t = 0;
		} 
		else if (strcmp(app->scene->allies[i]->charName.GetString(), "David el Enano") == 0)
		{
			
			app->render->DrawTexture(atkIcon, 50 + i * 150 - app->render->camera.x / scale, 25 - app->render->camera.y / scale);
			int atkNum = app->scene->allies[i]->attack;
			std::string atk = std::to_string(atkNum);
			app->fonts->BlitText(355, 50, Font, atk.c_str());
			app->render->DrawTexture(defIcon, 50 + i * 150 - app->render->camera.x / scale, 60 - app->render->camera.y / scale);
			int defNum = app->scene->allies[i]->defense;
			std::string def = std::to_string(defNum);
			app->fonts->BlitText(355, 85, Font, def.c_str());
			app->render->DrawTexture(mpIcon, 50 + i * 150 - app->render->camera.x / scale, 95 - app->render->camera.y / scale);
			int mpNum = app->scene->allies[i]->magicPower;
			std::string mp = std::to_string(mpNum);
			app->fonts->BlitText(355, 120, Font, mp.c_str());
			app->render->DrawTexture(dexIcon, 50 + i * 150 - app->render->camera.x / scale, 130 - app->render->camera.y / scale);
			int dexNum = app->scene->allies[i]->dexerity;
			std::string dex = std::to_string(dexNum);
			app->fonts->BlitText(355, 155, Font, dex.c_str());

			for (int h = 0; h < 3; ++h)
			{
				app->render->DrawTexture(itemBox, 135 + i * 150 - app->render->camera.x / scale, 30 + 45 * h - app->render->camera.y / scale);
			}

			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					app->render->DrawTexture(itemBox, 39 + i * 150 + k * 45 - app->render->camera.x / scale, 180 + j * 45 - app->render->camera.y / scale);

					if (t < app->scene->allies[i]->inventoryChar.Count())
					{
						if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
						{
							switch (app->scene->allies[i]->inventoryChar[t]->itemType)
							{
							case ItemType::HELMET:
								
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 - app->render->camera.y / scale;
								break;
							case ItemType::ARMOR:
								
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45 - app->render->camera.y / scale;
								break;
							case ItemType::WEAPON:
								
								app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
								app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90 - app->render->camera.y / scale;
								break;
							}
							
						}
						else {
							
							app->scene->allies[i]->inventoryChar[t]->position.x = 47 + i * 150 + k * 45 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 190 + j * 43 - app->render->camera.y / scale;
						}
						
						t++;
					}
				}

			}

			SDL_Rect healthBar;
			healthBar.x = 95 + i * 150 - app->render->camera.x / scale;
			healthBar.y = 120 - app->render->camera.y / scale;
			healthBar.w = 36;
			healthBar.h = 6;

			app->render->DrawTexture(Enano, 75 + i * 150 - app->render->camera.x / scale, 45 - app->render->camera.y / scale);

			int lifeW = (app->scene->allies[i]->life / app->scene->allies[i]->maxHP) * 36;
			if (lifeW <= 0)
			{
				lifeW = 0;
			}
			healthBar.w = lifeW;

			app->render->DrawRectangle(healthBar, 0, 255, 0, 255);

			app->render->DrawTexture(healthIcon, 75 + i * 150 - app->render->camera.x / scale, 75 - app->render->camera.y / scale);

			t = 0;
		}
		else if (strcmp(app->scene->allies[i]->charName.GetString(), "Eldrin") == 0)
		{

		app->render->DrawTexture(atkIcon, 50 + i * 150 - app->render->camera.x / scale, 25 - app->render->camera.y / scale);
		int atkNum = app->scene->allies[i]->attack;
		std::string atk = std::to_string(atkNum);
		app->fonts->BlitText(55, 50, Font, atk.c_str());
		app->render->DrawTexture(defIcon, 50 + i * 150 - app->render->camera.x / scale, 60 - app->render->camera.y / scale);
		int defNum = app->scene->allies[i]->defense;
		std::string def = std::to_string(defNum);
		app->fonts->BlitText(55, 85, Font, def.c_str());
		app->render->DrawTexture(mpIcon, 50 + i * 150 - app->render->camera.x / scale, 95 - app->render->camera.y / scale);
		int mpNum = app->scene->allies[i]->magicPower;
		std::string mp = std::to_string(mpNum);
		app->fonts->BlitText(55, 120, Font, mp.c_str());
		app->render->DrawTexture(dexIcon, 50 + i * 150 - app->render->camera.x / scale, 130 - app->render->camera.y / scale);
		int dexNum = app->scene->allies[i]->dexerity;
		std::string dex = std::to_string(dexNum);
		app->fonts->BlitText(55, 155, Font, dex.c_str());

		for (int h = 0; h < 3; ++h)
		{
			app->render->DrawTexture(itemBox, 135 + i * 150 - app->render->camera.x / scale, 30 + 45 * h - app->render->camera.y / scale);
		}

		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				app->render->DrawTexture(itemBox, 39 + i * 150 + k * 45 - app->render->camera.x / scale, 180 + j * 45 - app->render->camera.y / scale);

				if (t < app->scene->allies[i]->inventoryChar.Count())
				{
					if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
					{
						switch (app->scene->allies[i]->inventoryChar[t]->itemType)
						{
						case ItemType::HELMET:
							
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 - app->render->camera.y / scale;
							break;
						case ItemType::ARMOR:
							
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45 - app->render->camera.y / scale;
							break;
						case ItemType::WEAPON:
							
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90 - app->render->camera.y / scale;
							break;
						}

					}
					else {
						
						app->scene->allies[i]->inventoryChar[t]->position.x = 47 + i * 150 + k * 45 - app->render->camera.x / scale;
						app->scene->allies[i]->inventoryChar[t]->position.y = 190 + j * 43 - app->render->camera.y / scale;
					}

					t++;
				}
			}

		}

		SDL_Rect healthBar;
		healthBar.x = 95 + i * 150 - app->render->camera.x / scale;
		healthBar.y = 120 - app->render->camera.y / scale;
		healthBar.w = 36;
		healthBar.h = 6;

		app->render->DrawTexture(Eldrin, 75 + i * 150 - app->render->camera.x / scale, 45 - app->render->camera.y / scale);

		int lifeW = (app->scene->allies[i]->life / app->scene->allies[i]->maxHP) * 36;
		if (lifeW <= 0)
		{
			lifeW = 0;
		}
		healthBar.w = lifeW;

		app->render->DrawRectangle(healthBar, 0, 255, 0, 255);

		app->render->DrawTexture(healthIcon, 75 + i * 150 - app->render->camera.x / scale, 75 - app->render->camera.y / scale);

		t = 0;
		}
		else if (strcmp(app->scene->allies[i]->charName.GetString(), "Urgo el Bardo") == 0)
		{

		app->render->DrawTexture(atkIcon, 50 + i * 150 - app->render->camera.x / scale, 25 - app->render->camera.y / scale);
		int atkNum = app->scene->allies[i]->attack;
		std::string atk = std::to_string(atkNum);
		app->fonts->BlitText(505, 50, Font, atk.c_str());
		app->render->DrawTexture(defIcon, 50 + i * 150 - app->render->camera.x / scale, 60 - app->render->camera.y / scale);
		int defNum = app->scene->allies[i]->defense;
		std::string def = std::to_string(defNum);
		app->fonts->BlitText(505, 85, Font, def.c_str());
		app->render->DrawTexture(mpIcon, 50 + i * 150 - app->render->camera.x / scale, 95 - app->render->camera.y / scale);
		int mpNum = app->scene->allies[i]->magicPower;
		std::string mp = std::to_string(mpNum);
		app->fonts->BlitText(505, 120, Font, mp.c_str());
		app->render->DrawTexture(dexIcon, 50 + i * 150 - app->render->camera.x / scale, 130 - app->render->camera.y / scale);
		int dexNum = app->scene->allies[i]->dexerity;
		std::string dex = std::to_string(dexNum);
		app->fonts->BlitText(505, 155, Font, dex.c_str());

		for (int h = 0; h < 3; ++h)
		{
			app->render->DrawTexture(itemBox, 135 + i * 150 - app->render->camera.x / scale, 30 + 45 * h - app->render->camera.y / scale);
		}

		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				app->render->DrawTexture(itemBox, 39 + i * 150 + k * 45 - app->render->camera.x / scale, 180 + j * 45 - app->render->camera.y / scale);

				if (t < app->scene->allies[i]->inventoryChar.Count())
				{
					if (app->scene->allies[i]->inventoryChar[t]->isEquipped == true)
					{
						switch (app->scene->allies[i]->inventoryChar[t]->itemType)
						{
						case ItemType::HELMET:
							
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 - app->render->camera.y / scale;
							break;
						case ItemType::ARMOR:
							
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 45 - app->render->camera.y / scale;
							break;
						case ItemType::WEAPON:
							
							app->scene->allies[i]->inventoryChar[t]->position.x = 140 + i * 150 - app->render->camera.x / scale;
							app->scene->allies[i]->inventoryChar[t]->position.y = 30 + 90 - app->render->camera.y / scale;
							break;
						}

					}
					else {
						
						app->scene->allies[i]->inventoryChar[t]->position.x = 47 + i * 150 + k * 45 - app->render->camera.x / scale;
						app->scene->allies[i]->inventoryChar[t]->position.y = 190 + j * 43 - app->render->camera.y / scale;
					}

					t++;
				}
			}

		}

		SDL_Rect healthBar;
		healthBar.x = 95 + i * 150 - app->render->camera.x / scale;
		healthBar.y = 120 - app->render->camera.y / scale;
		healthBar.w = 36;
		healthBar.h = 6;

		app->render->DrawTexture(Bardo, 75 + i * 150 - app->render->camera.x / scale, 45 - app->render->camera.y / scale);

		int lifeW = (app->scene->allies[i]->life / app->scene->allies[i]->maxHP) * 36;
		if (lifeW <= 0)
		{
			lifeW = 0;
		}
		healthBar.w = lifeW;

		app->render->DrawRectangle(healthBar, 0, 255, 0, 255);

		app->render->DrawTexture(healthIcon, 75 + i * 150 - app->render->camera.x / scale, 75 - app->render->camera.y / scale);

		t = 0;
		}

		
	}

	

	if (((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) || (pad.l_x > 0 && app->selectActionCooldown == 0))
		&& itemSelectedIndex + 1 < app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() 
		&& app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex + 1]->isEquipped == false)
	{
		app->audio->PlayFx(app->scene->hoverFx);
		itemSelectedIndex += 1;
		app->selectActionCooldown = 15;
	}
	if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || (pad.l_x < 0 && app->selectActionCooldown == 0)) && itemSelectedIndex - 1 >= 0 && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex - 1]->isEquipped == false)
	{
		app->audio->PlayFx(app->scene->hoverFx);
		itemSelectedIndex -= 1;
		app->selectActionCooldown = 15;

	}
	if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || (pad.l_y > 0 && app->selectActionCooldown == 0)) && itemSelectedIndex + 3 < app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex + 3]->isEquipped == false)
	{
		app->audio->PlayFx(app->scene->hoverFx);
		itemSelectedIndex += 3;
		app->selectActionCooldown = 15;

	}
	else if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || (pad.l_y > 0 && app->selectActionCooldown == 0)))
	{

		int count = app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() - 1;
		if (app->scene->allies[inventorySelectedIndex]->inventoryChar[count]->isEquipped)
		{
			itemSelectedIndex = app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() - 2;
		}
		else {
			itemSelectedIndex = app->scene->allies[inventorySelectedIndex]->inventoryChar.Count() - 1;
		}
		app->selectActionCooldown = 15;

		app->audio->PlayFx(app->scene->hoverFx);
		
	}
	if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || (pad.l_y < 0 && app->selectActionCooldown == 0)) && itemSelectedIndex - 3 >= 0 && app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex - 3]->isEquipped == false)
	{
		app->audio->PlayFx(app->scene->hoverFx);
		itemSelectedIndex -= 3;
		app->selectActionCooldown = 15;

	}
	else if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || (pad.l_y < 0 && app->selectActionCooldown == 0)))
	{
		app->audio->PlayFx(app->scene->hoverFx);
		if (app->scene->allies[inventorySelectedIndex]->inventoryChar[0]->isEquipped)
		{
			itemSelectedIndex = 1;
		}
		else {
			itemSelectedIndex = 0;
		}
		app->selectActionCooldown = 15;

	}

	if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || (pad.a == 1 && app->selectActionCooldown == 0)) && app->scene->allies[inventorySelectedIndex]->inventoryChar.start != NULL)
	{
		app->selectActionCooldown = 20;


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
			if (strcmp(app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex]->itemName.GetString(), "Mage Item Quest") == 0)
			{
				if (inventorySelectedIndex != 0)
				{
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
				}
			}
			else
			{
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
			}
			
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

	
	
		if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || (pad.y == 1 && app->selectActionCooldown == 0)))
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
	
	if (app->selectActionCooldown > 0) {
		app->selectActionCooldown--;
	}


	currentArrowAnim = &idleArrowAnim;
	currentArrowAnim->Update();

	if (app->scene->allies[inventorySelectedIndex]->inventoryChar.start != NULL)
	{
		app->render->DrawTexture(itemText, 10 - app->render->camera.x / scale, 320 - app->render->camera.y / scale);
		app->fonts->BlitText(20, 340, Font, app->scene->allies[inventorySelectedIndex]->inventoryChar[itemSelectedIndex]->description.GetString());
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