#ifndef __BASEALLY_H__
#define __BASEALLY_H__

#include "Entity.h"
#include "Point.h"
#include "Dialogue.h"
#include "SString.h"
#include "List.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Item.h"

struct SDL_Texture;
class SString;


struct Ability
{
	int id;
	char* name;
};


class BaseAlly : public Entity
{
public:

	BaseAlly();
	virtual ~BaseAlly();

	virtual bool Awake();

	virtual bool Start();

	virtual bool Update(float dt);

	virtual bool PostUpdate();

	virtual bool CleanUp();

	virtual void OnCollision(PhysBody* physA, PhysBody* physB);

	virtual bool OnCollisionStay(PhysBody* physA, PhysBody* physB);

	virtual void InitDialogues();

	virtual void InitAnims();

	virtual void SetAttackAnimation();

	virtual void CheckAttack(int selectAttackIndex, int currentPlayerIndex);

	virtual void SetStats();

	virtual void SetHelmetStats();

	virtual void SetArmorStats();

	virtual void SetWeaponStats();

public:

	bool isPicked = false;
	List<SString> dialoguesNPC;

	//Attack Features
	bool isHighlighted = false;
	int numAttacks = 0;

	List<Ability> abilities;

	uint windowW, windowH;

	pugi::xml_node config;

	bool hasAttacked = false;

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;

	SString dialogueString;
	PhysBody* rangeDialogue;
	bool hasTalked = false;

	// ANIMATIONS
	Animation* currentAnim;

	Animation idleAnim;
	Animation attackAnim;

	//Counters for animation 
	int attackCounter = 0;

	SDL_Rect dialogueBoxPos;
	SDL_Rect dialogueBoxPos2;

	//Loading abilities
	int abilityId;
	const char* abilityName;
	SString abilityString;

	// Current stats of each character
	float life;
	float attack;
	float defense;
	float magicPower;
	float dexerity;
	float maxHP;

	// Base stats of each character
	float baseLife;
	float baseAttack;
	float baseDefense;
	float baseMagicPower;
	float baseDexerity;

	SString charName;

	// Debug Life
	SDL_Rect healthBar;

	// Items equipped by characters
	Item* helmetItem = NULL;
	bool isHelmetEquipped = false;
	Item* armorItem = NULL;
	bool isArmorEquipped = false;
	Item* weaponItem = NULL;
	bool isWeaponEquipped = false;

	List<Item*> inventoryChar;

	float damage;

private:


};

#endif // __BASEALLY_H__
