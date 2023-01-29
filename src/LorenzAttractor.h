#pragma once

#include "pch.h"
#include "Metaheuristic.h"

class LorenzAttractor : public Metaheuristic {
public:
    LorenzAttractor(const ParticlePropertys& particleProps, float a, float b, float c);
    ~LorenzAttractor() {}
   
    void update(float dT) override;

    void spawnRandomParticles(glm::vec2 xRand, glm::vec2 yRand, glm::vec2 zRand);
private:
    ParticlePropertys m_ParticleProps;

    struct Particle {
        Particle()
        : position(0.0f) {}

        glm::vec3 position;
        float rotation;
        std::vector<glm::vec3> previousPositions;
        std::vector<float> previousRotations;
    };

    std::vector<Particle> m_Particles;

    float a, b, c;

    void spawnParticle(const Particle& particle);
    void spawnParticleLine();
};
