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
    Uint8 r, g, b, a;
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
    void ParticleSystem::AddParticle(float x, float y, float xVel, float yVel, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int lifetime);

private:
    std::vector<Particle> particles;
    int numParticles = 50;
    SDL_Texture* waterTexture = nullptr;
};

#endif // __PARTICLESYSTEM_H__
