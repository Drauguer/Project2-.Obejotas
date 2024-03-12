#ifndef __DIALOGUE_H__
#define __DIALOGUE_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class DialogueType
{
	PLAYER,
	ITEM,
	NPC,
	ENEMY,
	UNKNOWN
};

class PhysBody;

class Dialogue
{
public:

	Dialogue() {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Dialogue::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Dialogue::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};



public:

	SString name;
	SString text;
	bool active = true;
	pugi::xml_node parameters;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;
	bool renderable = true;
};

#endif // __ENTITY_H__
