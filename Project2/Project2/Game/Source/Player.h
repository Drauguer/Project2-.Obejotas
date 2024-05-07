#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "BaseAlly.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Physics.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void InitAnims();

	bool SaveState(pugi::xml_node node, int num);

	bool LoadState(pugi::xml_node node, int num);

	void LoadNewMap(int mapID, int playerMapID);

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	const char* texturePath;
	pugi::xml_node config;
	uint texW, texH;
	

	//Audio fx
	int pickCoinFxId;
	unsigned int walkingRockFx;
	//Player Propierties
	int life = 30;
	int attack = 10;
	

	//Attack features
	bool isHighlighted;
	bool isOnPause = false;
	bool isPausing = false;
	int numAttacks = 2;

	//Animation overwolrd

	Animation frontWalk;
	Animation backWalk;
	Animation sideWalk;
	Animation* currentAnimation = nullptr;
	bool isFlipped = false;

	// Velocity
	b2Vec2 vel;

	// Is Talking
	bool isTalking = false;
};

#endif // __PLAYER_H__