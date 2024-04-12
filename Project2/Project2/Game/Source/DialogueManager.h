#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "Module.h"
#include "Dialogue.h"
#include "SString.h"
#include "List.h"

class DialogueManager : public Module
{
public:

	DialogueManager(bool startEnabled);

	// Destructor
	virtual ~DialogueManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Dialogue* CreateDialogue(SString text, DialogueType type);

	void DestroyDialogue(Dialogue* dialogue);

	void AddDialogue(Dialogue* dialogue);

public:

	List<Dialogue*> dialogueList;
	ListItem<Dialogue*>* item;
	bool testDialogue = false;
	bool isTalking = false;
	uint windowW, windowH;

	int Font = -1;

	bool activateCombat = false;

};

#endif // __DIALOGUEMANAGER_H__
