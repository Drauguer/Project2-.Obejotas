#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "Dialogue.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class NPC : public Entity
{
public:

	NPC();
	virtual ~NPC();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool OnCollisionStay(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;
	SString dialogueString;
	PhysBody* rangeDialogue;
	bool hasTalked = false;
	
};

#endif // __ITEM_H__
