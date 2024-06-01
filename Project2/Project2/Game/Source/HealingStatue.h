#ifndef __HEALINGSTATUE_H__
#define __HEALINGSTATUE_H__

#include "Entity.h"
#include "Point.h"
#include "Dialogue.h"
#include "SString.h"
#include "List.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
class SString;

class HealingStatue : public Entity
{
public:

	HealingStatue();
	virtual ~HealingStatue();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool OnCollisionStay(PhysBody* physA, PhysBody* physB);

	/*bool SaveState(pugi::xml_node node, int num);

	bool LoadState(pugi::xml_node node, int num);*/

public:

	bool isPicked = false;
	List<SString> dialoguesNPC;

	bool canHeal = true;

	Animation idleAnim;
	Animation* currentAnimation = nullptr;

	bool cantTalk = false;
	int timerTalk = 0;

private:

	SDL_Texture* statueOn;
	SDL_Texture* statueOff;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;
	SString dialogueString;
	PhysBody* range;



};

#endif // __HEALINGSTATUE_H__
