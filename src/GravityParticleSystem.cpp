#include "GravityParticleSystem.h"
#include "Utils.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"

GPS::GPS(const ParticlePropertys& particleProps, float* rotation) : Metaheuristic(particleProps), m_Rotation(rotation) {
    m_VerticesPerShape = 4 * Particle().previousPositions.size();
}

void GPS::update(float dT) {
    m_Translations.clear();

    std::vector<Particle> particles = m_Particles;
    float friction = 0.001;
    float magnitudeMinimum = 10;

    for (int i = 0; i < m_Particles.size(); i++) {
        float G = 1.0f;

        for (int j = 0; j < m_Particles.size(); j++) {
            glm::vec2 r = (m_Particles[j].position) - (m_Particles[i].position); 
            float magnitude = (sqrt(r.x * r.x + r.y * r.y));

            if (magnitude == 0) {
                continue;
            } 
            if (magnitude < magnitudeMinimum) {
                magnitude = magnitudeMinimum;
            }

            particles[i].velocity += (r / magnitude) * (G * (m_Particles[i].mass * m_Particles[j].mass) / (magnitude * magnitude));  // direction * magnitude
        }

        // particles[i].velocity += -(particles[i].velocity) * friction;

        glm::vec2 dV = particles[i].velocity;
        particles[i].position += dV;
        particles[i].previousPositions.push(particles[i].position);


        particles[i].rotation = atan(-dV.y / dV.x);
        particles[i].previousRotations.push(particles[i].rotation);

        if (particles[i].position.x > 960) {
            particles[i].position.x = 0;
        }
        if (particles[i].position.y > 590) {
            particles[i].position.y = 0;
        }
        if (particles[i].position.x < 0) {
            particles[i].position.x = 960;
        }
        if (particles[i].position.y < 0) {
            particles[i].position.y = 590;
        }

        for (int j = 0; j < particles[i].previousPositions.size(); j++) {
            glm::vec3 origin = glm::vec3(particles[i].previousPositions[j], 0.0);
            // m_Translations.emplace_back(glm::vec4(origin, particles[i].previousRotations[j]));
        };

        m_VerticesPerShape = 8;
        m_Translations.emplace_back(glm::vec4(100, 100, 0, *m_Rotation));
        m_Translations.emplace_back(glm::vec4(100, 200, 0, *m_Rotation));
    }

    m_Particles = particles;
}

void GPS::spawnParticle(const Particle& particle) {
    m_Particles.emplace_back(particle);
}

void GPS::spawnRandomParticles(glm::vec4 areaSize, float minSpeed, float maxSpeed) {
    for (int i = 0; i < m_ParticleProps.ammount; i++) {
        int negativeDirX = 1 + randNum(0, 1) * -2;
        int negativeDirY = 1 + randNum(0, 1) * -2;
        int speed = randNum((int)minSpeed, (int)maxSpeed);

        Particle particle;
        particle.position.x = randNum(areaSize[0], areaSize[1]);
        particle.position.y = randNum(areaSize[2], areaSize[3]);
        particle.mass = randNum(2, 5);
        particle.rotation = 0.0f;
        particle.velocity.x = ((float)randNum(0, 100) / 100) * negativeDirX;
        particle.velocity.y = ((float)randNum(0, 100) / 100) * negativeDirY;

        spawnParticle(particle);
    }
}
