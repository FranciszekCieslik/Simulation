#pragma once
#include "ofMain.h"

class Particle
{
public:
    ofVec3f position; // Pozycja cząsteczki
    ofVec3f velocity; // Prędkość cząsteczki
    ofColor color;    // Kolor cząsteczki
    float mass;       // Masa cząsteczki
    float radius;
    float lifeTime;

private:
    bool alive = true;

public:
    Particle(ofVec3f pos, ofVec3f vel, ofColor col, float mass, float radius, float lifeTime)
        : position(pos), velocity(vel), color(col), mass(mass), radius(radius), lifeTime(lifeTime)
    {
    }

    void update()
    {
        if (lifeTime <= 0)
        {
            alive = false;
        }
        if (alive)
        {
            // Zaktualizowanie pozycji na podstawie prędkości
            position += velocity;
            lifeTime -= 0.1f;
        }
    }

    void draw()
    {
        if (alive)
        {
            // ofLog() << "Drawing particle at position: " << position;
            ofSetColor(color);
            ofDrawSphere(position, radius);
        }
    }

    void draw(ofMaterial &material)
    {
        if (alive)
        {
            // ofLog() << "Drawing particle at position: " << position;
            material.setDiffuseColor(color); // Kolor rozproszony obiektu
            ofDrawSphere(position, radius);
        }
    
    }

    bool checkCollision(const Particle &p)
    {
        return (position.distance(p.position) < radius) || (position.distance(p.position) < p.radius);
    }

    bool isAlive()
    {
        return alive;
    }
};
