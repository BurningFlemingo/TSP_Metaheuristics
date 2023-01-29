#pragma once

#include "pch.h"
#include "Metaheuristic.h"

class GPS : public Metaheuristic {
public:
    GPS(const ParticlePropertys& particleProps, float* rotation);
    ~GPS() {}
   
    void update(float dT) override;

    void spawnRandomParticles(glm::vec4 areaSize, float minSpeed, float maxSpeed);
private:
    struct Particle {
        Particle()
        : position(0.0f), velocity(0.0f), size(1.0f), mass(1.0f) {}

        glm::vec2 position;
        glm::vec2 velocity;
        float rotation;
        float size;
        float mass;
        CircularBuffer<float, 25> previousRotations;
        CircularBuffer<glm::vec2, 25> previousPositions;
    };

    std::vector<Particle> m_Particles;
    float* m_Rotation;

    void spawnParticle(const Particle& particle);
};
