#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

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

	// Mira que ataque tiene que hacer el personaje
	void CheckAttack(int selectAttackIndex, int currentPlayerIndex);

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;
	

	//Audio fx
	int pickCoinFxId;

	//Player Propierties
	int life = 30;
	int attack = 10;
	

	//Attack features
	bool isHighlighted;
	int numAttacks = 2;


};

#endif // __PLAYER_H__