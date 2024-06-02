#include "ParticleSystem.h"
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
#include "Inventory.h"

#include<iostream>
#include<cstdlib>
#include <ctime>


ParticleSystem::ParticleSystem(bool startEnabled) : Module(startEnabled)
{
    name.Create("particleSystem");
}



bool ParticleSystem::Start() 
{
    waterTexture = app->tex->Load("Assets/Textures/agua.png");
    std::srand(std::time(nullptr));

    return true;
}

bool ParticleSystem::Update(float dt) {
    if ((app->scene->mapID == 0 || app->scene->mapID == 1 || app->scene->mapID == 5)&& !app->scene->player->isOnPause && !app->inventory->isInventory) {
        for (auto& p : particles) {
            p.y += p.yVel * dt;
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
            AddParticle(std::rand() % (app->win->width * 2), 0,
                0, 0.5, std::rand() % 200 + 50);
        }

        app->render->RenderParticles(particles, waterTexture);
    }
   
    return true;
}

bool ParticleSystem::CleanUp() {
    particles.clear();
    return true;
}

void ParticleSystem::MoreParticles(int number) {
    particles.resize(number);
    for (auto& p : particles) {
        p.x = std::rand() % (app->win->width);
        p.y = std::rand() % (app->win->height);
        p.xVel = (std::rand() % 100 - 50) / 10.0f;
        p.yVel = 0;
        p.lifetime = std::rand() % 100 + 50;
    }
}


void ParticleSystem::AddParticle(float x, float y, float xVel, float yVel, int lifetime) {
    Particle p;
    p.x = x;
    p.y = y;
    p.xVel = xVel;
    p.yVel = yVel;
    p.lifetime = lifetime;
    particles.push_back(p);
}

void ParticleSystem::Enable()
{
    if (!active)
    {
        active = true;

        Start();
    }
}


void ParticleSystem::Disable()
{
    if (active)
    {
        active = false;
        CleanUp();
    }
}

