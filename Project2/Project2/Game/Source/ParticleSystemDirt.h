#ifndef __PARTICLESYSTEMDIRT_H__
#define __PARTICLESYSTEMDIRT_H__



#include "Module.h"
#include "ParticleSystem.h"
#include <string>

#include <vector>

#include "SDL/include/SDL.h"

struct SDL_Texture;

class ParticleSystemDirt : public Module 
{
public:
    ParticleSystemDirt(bool startEnabled);
    ~ParticleSystemDirt();

   // bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();
    void MoreParticles(int number);
    void AddParticle(float x, float y, float xVel, float yVel, int lifetime);
    void Enable();

    void Disable();

public: 
    int positionX = 1;
    int positionY = 1;
    bool isRunning = false;
private:
    std::vector<Particle> particles;
    int numParticles = 50;
    SDL_Texture* dirtTexture = nullptr;
    
};

#endif // __PARTICLESYSTEMDIRT_H__
