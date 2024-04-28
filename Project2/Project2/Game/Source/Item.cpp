#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	

	return true;
}

bool Item::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	typeId = parameters.attribute("type").as_int();

	if (typeId == 1)
	{
		itemType = ItemType::HELMET;
	} 
	else if (typeId == 2)
	{
		itemType = ItemType::ARMOR;
	}
	else if (typeId == 3)
	{
		itemType = ItemType::WEAPON;
	}

	//initilize textures
	texture = app->tex->Load(texturePath);

	return true;
}

bool Item::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Item::CleanUp()
{
	return true;
}