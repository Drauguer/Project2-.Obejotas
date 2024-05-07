#ifndef __BASEENEMY_H__
#define __BASEENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Dialogue.h"
#include "SString.h"
#include "List.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "BaseAlly.h"


struct SDL_Texture;
class SString;


class BaseEnemy : public Entity
{
public:

	BaseEnemy();
	virtual ~BaseEnemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool OnCollisionStay(PhysBody* physA, PhysBody* physB);

	void InitDialogues();

	void InitAnims();

	virtual void SetAttackAnimation();


public:

	bool isPicked = false;
	List<SString> dialoguesNPC;

	// Stats de los enemigos
	float life;
	float attack;
	float defense;
	float magicPower;
	float dexerity;
	float maxHP;

	// Debug Life
	SDL_Rect healthBar;

	//Attack features 
	int numAttacks = 0;

	//Abilities
	List<Ability> abilities;
	bool hasAttacked = false;

	//Loading abilities
	int abilityId;
	const char* abilityName;
	SString abilityString;


	// ANIMATIONS
	Animation* currentAnim;

	Animation idleAnim;
	Animation attackAnim;

	

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;
	SString dialogueString;
	PhysBody* rangeDialogue;
	bool hasTalked = false;


};

#endif // __BASEENEMY_H__
