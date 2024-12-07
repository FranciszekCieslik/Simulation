#pragma once
#include "ofMain.h"
#include "Particle.hpp"

class Particles
{
public:
    ofVec3f Size;
    ofVec2f Radius;
    ofVec2f Mass;
    ofVec2f LifeTime;
    float maxVelocity;

    std::vector<Particle> particles;

public:
    Particles(size_t number, ofVec3f Size, ofVec2f Radius, ofVec2f Mass, ofVec2f LifeTime, float maxVelocity)
        : Size(Size), Radius(Radius), Mass(Mass), LifeTime(LifeTime), maxVelocity(maxVelocity)
    {
        for (size_t i = 0; i < number; i++)
        {
            addParticle();
        }
    }

    void addParticle()
    {
        particles.push_back(
            Particle(
                ofVec3f(ofRandom(0, Size.x), ofRandom(0, Size.y), ofRandom(0, Size.z)),
                ofVec3f(ofRandom(-maxVelocity, maxVelocity), ofRandom(-maxVelocity, maxVelocity), ofRandom(-maxVelocity, maxVelocity)),
                ofColor(ofRandom(50, 250), ofRandom(50, 250), ofRandom(50, 250)), ofRandom(Mass.x, Mass.y),
                ofRandom(Radius.x, Radius.y), ofRandom(LifeTime.x, LifeTime.y))

        );
    }

    void addParticle(size_t n)
    {
        for (size_t i{0}; i < n; i++)
            addParticle();
    }

    void draw()
    {
        for (auto &p : particles)
        {
            p.draw();
        }
    }

    void draw(ofMaterial &material)
    {
        for (auto &p : particles)
        {
            p.draw(material);
        }
    }

    void update()
    {
        // for (auto &p : particles)
        // {
        //     p.update();
        // }

        for (size_t i = 0; i < particles.size(); i++)
        {
            if (particles[i].isAlive())
            {
                particles[i].update();
            }
            else
            {
                particles.erase(particles.begin() + i);
            }
        }
    }

    void useMethod(std::function<void(Particle &)> func)
    {
        for (auto &p : particles)
        {
            func(p); // Wywołanie przekazanej funkcji na każdej cząsteczce
        }
    }

    size_t size()
    {
        return particles.size();
    }
};