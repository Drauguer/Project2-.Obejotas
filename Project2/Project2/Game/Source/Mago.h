#ifndef __MAGO_H__
#define __MAGO_H__

#include "Entity.h"
#include "BaseAlly.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Mago : public BaseAlly
{
public:

	Mago();

	virtual ~Mago();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	// Declare mago parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;


	//Audio fx
	int pickCoinFxId;
	unsigned int walkingRockFx;
	//Mago Propierties
	int life = 30;
	int attack = 20;


	//Attack features
	bool isHighlighted;
	int numAttacks = 2;

	//Animation overwolrd

	Animation frontWalk;
	Animation backWalk;
	Animation sideWalk;
	Animation* currentAnimation = nullptr;
	bool isFlipped = false;
};

#endif // __MAGO_H__
