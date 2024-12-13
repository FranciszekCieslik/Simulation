#pragma once
#include "ofMain.h"
#include "Particle.hpp"

class Particles
{
public:
    ofVec3f size_min;
    ofVec3f size_max;
    ofVec2f Radius;
    ofVec2f Mass;
    ofVec2f LifeTime;
    ofVec3f maxVelocity;
    ofVec3f color_max;
    ofVec3f color_min;

    std::vector<Particle> particles;

public:
    Particles(size_t number, ofVec3f size_min, ofVec3f size_max, ofVec2f Radius, ofVec2f Mass, ofVec2f LifeTime, ofVec3f maxVelocity, ofVec3f color_max, ofVec3f color_min)
        : size_min(size_min), size_max(size_max), Radius(Radius), Mass(Mass), LifeTime(LifeTime), maxVelocity(maxVelocity), color_max(color_max), color_min(color_min)
    {
        addParticle(number);
    }

    void addParticle()
    {
        particles.push_back(
            Particle(
                ofVec3f(ofRandom(size_min.x, size_max.x), ofRandom(size_min.y, size_max.y), ofRandom(size_min.z, size_max.z)),
                ofVec3f(ofRandom(-maxVelocity.x, maxVelocity.x), ofRandom(-maxVelocity.y, maxVelocity.y), ofRandom(-maxVelocity.z, maxVelocity.z)),
                ofColor(ofRandom(color_min.x, color_max.x), ofRandom(color_min.y, color_max.y), ofRandom(color_min.z, color_max.z)), ofRandom(Mass.x, Mass.y),
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