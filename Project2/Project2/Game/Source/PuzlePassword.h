#ifndef __PUZLEPASSWORD_H__
#define __PUZLEPASSWORD_H__

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

class PuzlePassword : public Entity
{
public:

	PuzlePassword();
	virtual ~PuzlePassword();

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

	void SetCombinations();

	void DrawNumbers();
	
	bool IsCombinationCorrect();


	int RandomNumber(int min, int max);

	// Función para dibujar la barra de tiempo
	void DrawProgressBar();

public:


	SDL_Texture* arrowTexture = NULL;
	const char* arrowTexturePath;
	Animation* currentArrowAnim;

	Animation idleArrowAnim;

	bool  hasLost = false;

	bool canInteract;
	bool isActive = true;
	bool isTriggered = false;

	bool hasSolvedPuzle = false;

	int id;

	bool isPicked = false;

	int Font = -1;

	uint windowW, windowH;

	pugi::xml_node config;

	SDL_Texture* texture;
	SDL_Texture* solutionTexture;
	const char* texturePath;
	uint texW, texH;
	const char* dialogueChar;



	// ANIMATIONS
	Animation* currentAnim;

	List<int> solutionIds;

	SString text;

	int currentIndex = 0;

	List<int> currentCombination;
	List<int> finalCombination;

private:


};

#endif // __PUZLEPASSWORD_H__
