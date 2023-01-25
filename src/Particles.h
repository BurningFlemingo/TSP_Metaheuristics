#pragma once

#include "pch.h"
#include "Renderer/Shapes.h"
#include "Renderer/Renderer.h"
#include "Core/EventHandler.h"

struct Particle {
    Quad quad;
    glm::vec2 direction;
    float speed;
    float decay;
};

class ParticleSystem : public System {
public:
    ~ParticleSystem() {}

    ParticleSystem(AppState& appState);
    void createParticle(Particle particle);
    void createRandomParticles(glm::vec2 areaSize, uint ammount, float minSpeed, float maxSpeed);
    void draw() override;
    void update(float dT) override;
private:
    uint m_ParticleVAO;
    uint m_ProgramID;

    AppState& m_AppState;

    std::vector<Particle> m_Particles;

    void glSetup();
};

// void createParticle(uint programID, Particle particle);
// void updateParticles(uint programID);
// void drawParticles(uint programID);
// 
// Fun-ctions (:
// void createRandomParticles(uint programID, uint xAreaSize, uint yAreaSize, uint number, float minSpeed, float maxSpeed);

