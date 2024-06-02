#include "ParticleSystem.h"
#include "ParticleSystemFire.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "BattleScene.h"
#include "Window.h"

#include<iostream>
#include<cstdlib>
#include <ctime>



ParticleSystemBug::ParticleSystemBug(bool startEnabled) : Module(startEnabled)
{
    name.Create("particleSystem");
}



bool ParticleSystemBug::Start()
{
    bugTexture = app->tex->Load("Assets/Textures/dragonFly.png");
    bugAnim.PushBack({ 0, 0, 32, 32 });
    bugAnim.PushBack({ 32, 0, 32, 32 });
    bugAnim.PushBack({ 32*2, 0, 32, 32 });
    bugAnim.PushBack({ 32*3, 0, 32, 32 });
    bugAnim.loop = true;
    bugAnim.speed = 0.2;
    std::srand(std::time(nullptr));
    currentAnim = &bugAnim;

    return true;
}

bool ParticleSystemBug::Update(float dt) {
        for (auto& p : particles) {
            p.y += p.yVel * dt;
            p.x += p.xVel * dt;
            p.lifetime--;

            if (p.lifetime <= 0) {

                // Remove particles with lifetime <= 0
                particles.erase(std::remove_if(particles.begin(), particles.end(),
                    [](const Particle& p) { return p.lifetime <= 0; }),
                    particles.end());
            }
        }
        // Add new particles to maintain the number of particles
        while (particles.size() < numParticles) {
            AddParticle(std::rand() % (app->win->width /2), (std::rand() % app->win->height)+ (app->win->height /8),
                GenerateRandomFloat(), 0, std::rand() % 200 + 50);
        }
        currentAnim->Update();
        app->render->RenderParticlesWithAnim(particles, bugTexture, &currentAnim->GetCurrentFrame());
    
   
    return true;
}

bool ParticleSystemBug::CleanUp() {
    particles.clear();
    return true;
}

void ParticleSystemBug::MoreParticles(int number) {
    particles.resize(number);
    for (auto& p : particles) {
        p.x = std::rand() % (app->win->width);
        p.y = std::rand() % (app->win->height);
        p.xVel = (std::rand() % 100 - 50) / 10.0f;
        p.yVel = 0;
        p.lifetime = std::rand() % 100 + 50;
    }
}


void ParticleSystemBug::AddParticle(float x, float y, float xVel, float yVel, int lifetime) {
    Particle p;
    p.x = x;
    p.y = y;
    p.xVel = xVel;
    p.yVel = yVel;
    p.lifetime = lifetime;
    particles.push_back(p);

}

void ParticleSystemBug::Enable()
{
    if (!active)
    {
        active = true;

        Start();
    }
}

float ParticleSystemBug::GenerateRandomFloat()
{
   int number =  rand() % 5;
   switch (number)
   {
   case 0:
       return 0.05;
       break; 
   case 1:
       return 0.1;
       break; 
   case 2:
       return 0.15;
       break;
   case 3:
       return -0.15;
       break; 
   case 4:
       return -0.1;
       break; 
   case 5:
       return -0.05;
       break; 
   default:
       break;
   }

}


void ParticleSystemBug::Disable()
{
    if (active)
    {
        active = false;
        CleanUp();
    }
}

