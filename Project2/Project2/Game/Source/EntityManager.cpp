#include "EntityManager.h"
#include "Player.h"
#include "Mago.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "BaseEnemy.h"
#include "NPC.h"
#include "BaseAlly.h"
#include "PuzlePilar.h"


#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::MAGO:
		entity = new Mago();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::NPC:
		entity = new NPC();
		break;
	case EntityType::ENEMY:
		entity = new BaseEnemy();
		break;
	case EntityType::ALLY:
		entity = new BaseAlly();
		break;
	case EntityType::PUZLEPILAR:
		entity = new PuzlePilar();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

bool EntityManager::PostUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->PostUpdate();
	}

	return ret;
}

void EntityManager::Enable()
{
	if (!active)
	{
		active = true;

		Start();
	}
}

void EntityManager::Disable()
{
	if (active)
	{
		active = false;
		CleanUp();
	}
}

bool EntityManager::SaveState(pugi::xml_node node)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	pugi::xml_node player = node.append_child("Player");
	pugi::xml_node npcs = node.append_child("NPCs");
	pugi::xml_node enemies = node.append_child("Enemies");
	pugi::xml_node allies = node.append_child("Allies");

	int playerNo = 1;
	int npcNo = 1;
	int enemyNo = 1;
	int allyNo = 1;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;
		EntityType type = item->data->type;
		switch (type)
		{
		case EntityType::PLAYER:
			ret = pEntity->SaveState(player, playerNo);
			playerNo++;
			break;
		case EntityType::NPC:
			ret = pEntity->SaveState(npcs, npcNo);
			npcNo++;
			break;
		case EntityType::ENEMY:
			ret = pEntity->SaveState(enemies, enemyNo);
			enemyNo++;
			break;
		case EntityType::ALLY:
			ret = pEntity->SaveState(allies, allyNo);
			allyNo++;
			break;
		default:
			break;
		}
	}

	return ret;
}


bool EntityManager::LoadState(pugi::xml_node node)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	pugi::xml_node player = node.child("Player");
	pugi::xml_node npcs = node.child("NPCs");
	pugi::xml_node enemies = node.child("Enemies");
	pugi::xml_node allies = node.child("Allies");

	int playerNo = 1;
	int npcNo = 1;
	int enemyNo = 1;
	int allyNo = 1;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;
		EntityType type = item->data->type;
		switch (type)
		{
		case EntityType::PLAYER:
			ret = pEntity->LoadState(player, playerNo);
			playerNo++;
			break;
		case EntityType::NPC:
			ret = pEntity->LoadState(npcs, npcNo);
			npcNo++;
			break;
		case EntityType::ENEMY:
			ret = pEntity->LoadState(enemies, enemyNo);
			enemyNo++;
			break;
		case EntityType::ALLY:
			ret = pEntity->LoadState(allies, allyNo);
			allyNo++;
			break;
		default:
			break;
		}
	}

	return ret;
}