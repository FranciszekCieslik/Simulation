#pragma once
#include "ofMain.h"
#include "Particles.hpp"
#include "Particle.hpp"

class Force {
public:
    // Konstruktor przyjmujący równanie obliczające siłę i warunek aktywacji
    Force(std::function<ofVec3f(const Particle&)> forceFunction,
          std::function<bool(const Particle&)> conditionFunction)
        : computeForce(forceFunction), isActive(conditionFunction) {}

    // Metoda stosująca siłę do cząsteczki
    void applyTo(Particle& particle) {
        if (isActive(particle)) { // Jeśli warunek jest spełniony
            ofVec3f acceleration = computeForce(particle) / particle.mass; // F = ma => a = F/m
            particle.velocity += acceleration; // Aktualizacja prędkości cząsteczki
        }
    }

private:
    std::function<ofVec3f(const Particle&)> computeForce;  // Równanie siły
    std::function<bool(const Particle&)> isActive;         // Warunek aktywacji
};