#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__



#include "Module.h"
#include <string>

#include <vector>

#include "SDL/include/SDL.h"

struct SDL_Texture;
struct Particle {
    float x, y;
    float xVel, yVel;
    int lifetime;
};

class ParticleSystem : public Module 
{
public:
    ParticleSystem(bool startEnabled);
    ~ParticleSystem();

   // bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();
    void MoreParticles(int number);
    void AddParticle(float x, float y, float xVel, float yVel, int lifetime);
    void Enable();

    void Disable();

private:
    std::vector<Particle> particles;
    int numParticles = 100;
    SDL_Texture* waterTexture = nullptr;
};

#endif // __PARTICLESYSTEM_H__
