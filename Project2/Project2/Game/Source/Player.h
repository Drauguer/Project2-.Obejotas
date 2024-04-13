#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "BaseAlly.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

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
	int numAttacks = 2;

	//Animation overwolrd

	Animation frontWalk;
	Animation backWalk;
	Animation sideWalk;
	Animation* currentAnimation = nullptr;
	bool isFlipped = false;
};

#endif // __PLAYER_H__