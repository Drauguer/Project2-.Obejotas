#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "Dialogue.h"
#include "SString.h"
#include "List.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
class SString;

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

	bool SaveState(pugi::xml_node node, int num);

	bool LoadState(pugi::xml_node node, int num);

	void InitDialogues();

	void CheckQuest(int questID);

public:

	bool isPicked = false;
	List<SString> dialoguesNPC;

	bool hasCombat;
	bool triggerInRange;
	int mapID;
	bool hasTalked = false;
	int npcID;
	bool hasQuest;
	bool QuestCompleted = false;
	int QuestID = 999;
	int itemRewardID = 999;

	Animation idleAnim;
	Animation* currentAnimation = nullptr;

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;
	SString dialogueString;
	PhysBody* rangeDialogue;
	
	
	
};

#endif // __NPC_H__
