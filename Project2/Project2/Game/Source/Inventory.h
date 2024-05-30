#ifndef __INVENTORY_H__
#define __DIALOGUEMANAGER_H__

#include "Module.h"
#include "Item.h"
#include "SString.h"
#include "List.h"
#include "Animation.h"

class Inventory : public Module
{
public:

	Inventory(bool startEnabled);

	// Destructor
	virtual ~Inventory();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Enable Module
	void Enable();

	// Disable Module
	void Disable();

public:

	uint windowW, windowH;

	int Font = -1;

	// Characters Textures
	SDL_Texture* Eldrin;
	SDL_Texture* Mago;
	SDL_Texture* Enano;
	SDL_Texture* Bardo;

	// Inventory UI
	SDL_Texture* itemBox;
	SDL_Texture* charBox;

	SDL_Texture* atkIcon;
	SDL_Texture* defIcon;
	SDL_Texture* mpIcon;
	SDL_Texture* dexIcon;

	SDL_Texture* healthIcon;

	// Text Box
	SDL_Texture* itemText;

	// Arrow
	SDL_Texture* arrowTexture = NULL;
	const char* arrowTexturePath;
	Animation* currentArrowAnim = NULL;

	Animation idleArrowAnim;

	// Index
	int inventorySelectedIndex = 0;
	int itemSelectedIndex = 0;

	// Item for changing
	Item* changeItem = NULL;

	bool isInventory = false;

};

#endif // __DIALOGUEMANAGER_H__
