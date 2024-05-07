#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class EntityType
{
	PLAYER,
	MAGO,
	ITEM,
	NPC,
	ALLY,
	ENEMY,
	PUZLE_PILAR,
	PUZLE_PASSWORD,
	UNKNOWN
};

class PhysBody;

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

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

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node node, int num)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node node, int num)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual bool OnCollisionStay(PhysBody* physA, PhysBody* physB) {
		return true;
	};



public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 
	PhysBody* pbody;



	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;

	bool isHighlighted;
};

#endif // __ENTITY_H__