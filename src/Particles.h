#pragma once

#include "pch.h"
#include "Renderer/Shapes.h"
#include "Renderer/Renderer.h"
#include "Core/EventHandler.h"
#include "Utils.h"

struct Particle {
    glm::vec2 position;
    glm::vec2 direction;
    float speed;
    float decay;
    CircularBuffer<glm::vec2, 50> previousPositions;
};

class ParticleSystem : public System {
public:
    ~ParticleSystem() {}

    ParticleSystem(AppState& appState);
    void glSetup();

    void createRandomParticles(glm::vec2 areaSize, uint ammount, float minSpeed, float maxSpeed, uint particleSize);

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

    void createParticle(Particle particle);
};

// void createParticle(uint programID, Particle particle);
// void updateParticles(uint programID);
// void drawParticles(uint programID);
// 
// Fun-ctions (:
// void createRandomParticles(uint programID, uint xAreaSize, uint yAreaSize, uint number, float minSpeed, float maxSpeed);

