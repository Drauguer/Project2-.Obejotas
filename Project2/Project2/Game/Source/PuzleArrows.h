#ifndef __PUZLEARROWS_H__
#define __PUZLEARROWS_H__

#include "Entity.h"
#include "Point.h"
#include "Dialogue.h"
#include "SString.h"
#include "List.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Item.h"

struct SDL_Texture;
class SString;

class PuzleArrows : public Entity
{
public:

	PuzleArrows();
	virtual ~PuzleArrows();

	virtual bool Awake();

	virtual bool Start();

	virtual bool Update(float dt);

	virtual bool PostUpdate();

	virtual bool CleanUp();

	virtual void OnCollision(PhysBody* physA, PhysBody* physB);

	virtual bool OnCollisionStay(PhysBody* physA, PhysBody* physB);

	void InitAnims();

	void GeneratePassword();
	
	void CheckResult();


	int RandomNumber(int min, int max);

	// Función para dibujar la barra de tiempo
	void DrawProgressBar();

public:
	int sequenceCounter = 2;
	SDL_Rect barRect; // Rectángulo para la barra de tiempo
	float progress = 200;   // Progreso de la barra de tiempo

	int barWidth;

	bool  hasLost = false;

	const int MAX_BAR_WIDTH = 400; // Ancho máximo de la barra de tiempo
	const float MAX_PROGRESS = 100.0f; // Progreso máximo de la barra de tiempo

	bool hasCreatedPassword = false;

	bool canInteract;
	bool isActive = true;
	bool isTriggered = false;
	bool hasPressedE =  false;

	int id;

	bool isPicked = false;

	//Attack Features

	uint windowW, windowH;

	pugi::xml_node config;

	bool hasAttacked = false;

	SDL_Texture* arrowsTexture;
	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;

	// ANIMATIONS
	Animation* currentAnim;

	Animation leftArrow;
	Animation upArrow;
	Animation rightArrow;
	Animation downArrow;


	List<int> solutionIds;

	SString charName;

private:


};

#endif // __PUZLEARROWS_H__
