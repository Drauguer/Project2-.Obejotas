#ifndef __BASEALLY_H__
#define __BASEALLY_H__

#include "Entity.h"
#include "Point.h"
#include "Dialogue.h"
#include "SString.h"
#include "List.h"
#include "SDL/include/SDL.h"

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

	virtual bool CleanUp();

	virtual void OnCollision(PhysBody* physA, PhysBody* physB);

	virtual bool OnCollisionStay(PhysBody* physA, PhysBody* physB);

	virtual void InitDialogues();

	virtual void CheckAttack(int selectAttackIndex, int currentPlayerIndex);

public:

	bool isPicked = false;
	List<SString> dialoguesNPC;

	//Attack Features
	bool isHighlighted;
	int numAttacks = 2;

	List<Ability> abilities;

	pugi::xml_node config;

	bool hasAttacked = false;

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;

	SString dialogueString;
	PhysBody* rangeDialogue;
	bool hasTalked = false;


	//Loading abilities
	int abilityId;
	const char* abilityName;
	SString abilityString;

private:

	




	


};

#endif // __BASEALLY_H__
