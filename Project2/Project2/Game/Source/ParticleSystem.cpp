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


ParticleSystem::ParticleSystem(bool startEnabled) : Module(startEnabled)
{
    name.Create("particleSystem");
}



bool ParticleSystem::Start() 
{
    waterTexture = app->tex->Load("Assets/Textures/agua.png");
    particles.resize(numParticles);
    std::srand(std::time(nullptr));

    for (auto& p : particles) {
        p.x = std::rand() %  (app->win->width);
        p.y = 0;
        p.xVel = 0;
        p.yVel = 0.5;
        p.r = std::rand() % 256;
        p.g = std::rand() % 256;
        p.b = std::rand() % 256;
        p.a = 255;
        p.lifetime = std::rand() % 200 + 50;
    }
    return true;
}

bool ParticleSystem::Update(float dt) {
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
        AddParticle(std::rand() % (app->win->width), 0,
            0, 0.5,
            std::rand() % 256, std::rand() % 256, std::rand() % 256, 255, std::rand() % 200 + 50);
    }

    app->render->RenderParticles(particles, waterTexture);
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
        p.r = std::rand() % 256;
        p.g = std::rand() % 256;
        p.b = std::rand() % 256;
        p.a = 255;
        p.lifetime = std::rand() % 100 + 50;
    }
}


void ParticleSystem::AddParticle(float x, float y, float xVel, float yVel, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int lifetime) {
    Particle p;
    p.x = x;
    p.y = y;
    p.xVel = xVel;
    p.yVel = yVel;
    p.r = r;
    p.g = g;
    p.b = b;
    p.a = a;
    p.lifetime = lifetime;
    particles.push_back(p);
}