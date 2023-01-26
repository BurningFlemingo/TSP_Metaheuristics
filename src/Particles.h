#pragma once

#include "pch.h"
#include "Renderer/Shapes.h"
#include "Renderer/Renderer.h"
#include "Core/EventHandler.h"
#include "Utils.h"

struct Particle {
    float lifetime;
    glm::vec2 position;
    glm::vec2 forces;
    float mass;
    CircularBuffer<glm::vec2, 50> previousPositions;
};

class ParticleSystem : public System {
public:
    ~ParticleSystem() {}

    ParticleSystem(AppState& appState);
    void glSetup();

    void emitter(glm::vec4 areaSize, uint spawnRate, float lifetime);
    void spawnRandomParticles(glm::vec4 areaSize, uint ammount, float minSpeed, float maxSpeed, uint particleSize);

    void draw() override;
    void update(float dT) override;
private:
    uint m_ParticleVAO;
    uint m_ProgramID;
    uint m_InstanceVBO;

    AppState& m_AppState;

    std::vector<Particle> m_Particles;
    uint m_ParticleSize;
    std::vector<glm::vec3> m_Translations;

    void spawnParticle(Particle particle);
};

// void createParticle(uint programID, Particle particle);
// void updateParticles(uint programID);
// void drawParticles(uint programID);
// 
// Fun-ctions (:
// void createRandomParticles(uint programID, uint xAreaSize, uint yAreaSize, uint number, float minSpeed, float maxSpeed);

