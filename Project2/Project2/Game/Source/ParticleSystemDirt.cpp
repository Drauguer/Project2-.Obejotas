#include "ParticleSystemDirt.h"
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

#include<iostream>
#include<cstdlib>
#include <ctime>


ParticleSystemDirt::ParticleSystemDirt(bool startEnabled) : Module(startEnabled)
{
    name.Create("particleSystem");
}


bool ParticleSystemDirt::Start()
{
    dirtTexture = app->tex->Load("Assets/Textures/dirt.png");
    std::srand(std::time(nullptr));
    return true;
}

bool ParticleSystemDirt::Update(float dt) {
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
        AddParticle((rand() %40) + positionX, (rand() % 40) +positionY,
            0, 0,
            std::rand() % 20+ 10);
    }

    app->render->RenderParticles(particles, dirtTexture);
    return true;
}

bool ParticleSystemDirt::CleanUp() {
    particles.clear();
    return true;
}


void ParticleSystemDirt::AddParticle(float x, float y, float xVel, float yVel,  int lifetime) {
    Particle p;
    p.x = x;
    p.y = y;
    p.xVel = xVel;
    p.yVel = yVel;
    p.lifetime = lifetime;
    particles.push_back(p);
}

void ParticleSystemDirt::Enable()
{
    if (!active)
    {
        isRunning = true;
        active = true;

        Start();
    }
}


void ParticleSystemDirt::Disable()
{
    if (active)
    {
        isRunning = false;
        active = false;
        CleanUp();
    }
}

