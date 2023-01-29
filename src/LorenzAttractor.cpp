#include "LorenzAttractor.h"
#include "Utils.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"

LorenzAttractor::LorenzAttractor(const ParticlePropertys& particleProps, float a, float b, float c)
    : Metaheuristic(particleProps), m_ParticleProps(particleProps), a(1), b(1), c(1) {
    m_VerticesPerShape = 4;
    spawnParticleLine();
}

void LorenzAttractor::update(float dT) {
    m_Translations.clear();

    for (int i = 0; i < m_Particles.size(); i++) {
        Particle& particle = m_Particles[i];
        float scale = 100;

        float x = particle.position.x;
        float y = particle.position.y;
        float z = particle.position.z;

        float dx = (a * (y - x)) * dT;
        float dy = (x * (b - z) - y) * dT;
        float dz = (x * y - c * z) * dT;

        glm::vec3 difference = particle.position - glm::vec3(dx, dy, dz);
        particle.position += glm::vec3(dx, dy, dz);
        particle.previousPositions.emplace_back(particle.position);

        float radians = atan(-difference.y / difference.x);
        particle.rotation = (radians * 180) / glm::pi<float>();
        particle.previousRotations.emplace_back(particle.rotation);

        print("ROT");
        print(particle.rotation);

        glm::vec3 pos = particle.position;
        std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;

        for (int i = 0; i < particle.previousPositions.size(); i++) {
            glm::vec3 origin = glm::vec3(particle.previousPositions[i] * glm::vec3(scale)) + glm::vec3(1920.0 / 4.0, 1080.0 / 4.0, 0.0);
            m_Translations.emplace_back(glm::vec4(origin, particle.previousRotations[i]));
        }
    }
    m_VerticesPerShape = 4 * m_Particles[0].previousPositions.size();
}

void LorenzAttractor::spawnParticle(const Particle& particle) {
    m_Particles.emplace_back(particle);
}

void LorenzAttractor::spawnRandomParticles(glm::vec2 xRand, glm::vec2 yRand, glm::vec2 zRand) {

    for (int i = 0; i < m_ParticleProps.ammount; i++) {
        Particle particle;
        // particle.position.x = 0.01 * randNum(areaSize[0], areaSize[1]);
        // particle.position.y = 0 * randNum(areaSize[2], areaSize[3]);
        glm::vec3 negative((randNum(0, 1) * -2) + 1);
        particle.position.x = randNum(xRand.x, xRand.y) * negative.x;
        particle.position.y = randNum(yRand.x, yRand.y) * negative.y;
        particle.position.z = randNum(zRand.x, zRand.y) * negative.z;

        particle.rotation = 0.0f;

        spawnParticle(particle);
    }
}


void LorenzAttractor::spawnParticleLine() {
    for (int i = 0; i < m_ParticleProps.ammount; i++) {
        Particle particle;
        int negative = 1;
        if (i < (m_ParticleProps.ammount / 2)) {
            negative = -1;
        }
        particle.position.x = 0.1;
        particle.position.y = 0.1;
        particle.position.z = 1;

        particle.rotation = 0.0f;

        spawnParticle(particle);
    }
}
