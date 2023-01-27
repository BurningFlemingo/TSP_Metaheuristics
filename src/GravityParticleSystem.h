#pragma once

#pragma once
#include "pch.h"
#include "Metaheuristic.h"

class GPS : public Metaheuristic {
public:
    GPS(const ParticlePropertys& particleProps);
    ~GPS() {}
   
    void update(float dT) override;

    void spawnRandomParticles(glm::vec4 areaSize, float minSpeed, float maxSpeed);
private:
    struct Particle {
        Particle()
        : position(0.0f), velocity(0.0f), color(1.0f), size(1.0f), mass(1.0f) {}

        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color;
        float size;
        float mass;
        CircularBuffer<glm::vec2, 50> previousPositions;
    };

    std::vector<Particle> m_Particles;

    void spawnParticle(const Particle& particle);
};
