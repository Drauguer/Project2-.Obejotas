#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum class ItemType
{
	HELMET,
	ARMOR,
	WEAPON,
	NUM,
};

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;
	bool isEquipped = false;

	int typeId = 0;
	ItemType itemType;
	SString itemName;

	float bonusATK = 0;
	float bonusDEF = 0;
	float bonusMP = 0;
	float bonusDEX = 0;
	float bonusHP = 0;

	bool newInInventory = false;

	SString description;

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
};

#endif // __ITEM_H__