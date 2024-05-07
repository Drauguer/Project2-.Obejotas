#ifndef __PUZLEPILAR_H__
#define __PUZLEPILAR_H__

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

class PuzlePilar : public Entity
{
public:

	PuzlePilar();
	virtual ~PuzlePilar();

	virtual bool Awake();

	virtual bool Start();

	virtual bool Update(float dt);

	virtual bool PostUpdate();

	virtual bool CleanUp();

	virtual void OnCollision(PhysBody* physA, PhysBody* physB);

	virtual bool OnCollisionStay(PhysBody* physA, PhysBody* physB);

	void InitAnims();

public:

	bool canInteract;
	bool isActive = true;
	bool isTriggered = false;
	bool hasPressedE =  false;

	int id;

	bool isPicked = false;

	//Attack Features

	uint windowW, windowH;

	pugi::xml_node config;

	bool hasAttacked = false;

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;

	// ANIMATIONS
	Animation* currentAnim;

	Animation idleAnim;
	Animation triggeredAnim;

	SString charName;

private:


};

#endif // __PUZLEPILAR_H__
