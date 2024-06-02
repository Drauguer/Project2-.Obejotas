#ifndef __PARTICLESYSTEMFIRE_H__
#define __PARTICLESYSTEMFIRE_H__

#include "ParticleSystem.h"


#include "Module.h"
#include <string>

#include <vector>

#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

struct ParticleWithAnim {
    float x, y;
    float xVel, yVel;
    int lifetime;
};
class ParticleSystemBug : public Module 
{
public:
    ParticleSystemBug(bool startEnabled);
    ~ParticleSystemBug();

   // bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();
    void MoreParticles(int number);
    void AddParticle(float x, float y, float xVel, float yVel, int lifetime);
    void Enable();

    void Disable();

    float GenerateRandomFloat();

private:
    std::vector<Particle> particles;
    int numParticles = 10;
    SDL_Texture* bugTexture = nullptr;
    Animation bugAnim;
    Animation* currentAnim;

    //lluvia 
    
};

#endif // __PARTICLESYSTEMFIRE_H__
