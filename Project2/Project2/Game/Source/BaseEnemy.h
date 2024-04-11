#ifndef __BASEENEMY_H__
#define __BASEENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Dialogue.h"
#include "SString.h"
#include "List.h"
#include "SDL/include/SDL.h"

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

public:

	bool isPicked = false;
	List<SString> dialoguesNPC;

	// Stats de los enemigos
	int life;
	int attack;
	int defense;
	int magicPower;
	int dexerity;

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
