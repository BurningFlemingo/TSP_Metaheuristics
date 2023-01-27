#include "GravityParticleSystem.h"
#include "Utils.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"

GPS::GPS(const ParticlePropertys& particleProps) : Metaheuristic(particleProps) {
    m_VerticesPerShape = 4 * Particle().previousPositions.size();
}

void GPS::update(float dT) {
    m_Translations.clear();

    std::vector<Particle> particles = m_Particles;
    float friction = 0.001;

    for (int i = 0; i < m_Particles.size(); i++) {
        float G = 1.0f;

        for (int j = 0; j < m_Particles.size(); j++) {
            glm::vec2 r = (m_Particles[j].position) - (m_Particles[i].position); 
            float magnitude = (sqrt(r.x * r.x + r.y * r.y));
            float m1 = m_Particles[i].mass;
            float m2 = m_Particles[j].mass;

            glm::vec2 negative(1, 1);
            if (r.x < 0) {
                negative.x = -1;
            }
            if (r.y < 0) {
                negative.y = -1;
            }
            if (magnitude == 0) {
                continue;
            } 
            if (magnitude < 10) {
                magnitude = 10;
            }

            particles[i].velocity += -negative * G * (m1 * m2) / (magnitude * magnitude);
        }

        particles[i].velocity += -(particles[i].velocity) * friction;

        particles[i].position += particles[i].velocity;
        particles[i].previousPositions.push(particles[i].position);

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
            m_Translations.emplace_back(glm::vec3(particles[i].previousPositions[j] - glm::vec2((float)m_ParticleProps.size / 2), 3.0f / (j + 0.01)));
        };
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
        particle.velocity.x = ((float)randNum(0, 100) / 100) * negativeDirX;
        particle.velocity.y = ((float)randNum(0, 100) / 100) * negativeDirY;

        spawnParticle(particle);
    }
}
