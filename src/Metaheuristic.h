#pragma once

#include "pch.h"
#include "Renderer/Shapes.h"
#include "Renderer/Renderer.h"
#include "Core/EventHandler.h"
#include "Utils.h"

struct ParticlePropertys {
    ParticlePropertys(uint p_Ammount, float p_Size) : ammount(p_Ammount), size(p_Size) {}
    uint ammount;
    float size;
};

class Metaheuristic {
public:
    Metaheuristic(const ParticlePropertys& particleProps);

    virtual void update(float dT) = 0;
    void draw();
protected:
    ParticlePropertys m_ParticleProps;

    uint m_ParticleVAO;
    uint m_ProgramID;
    uint m_InstanceVBO;

    uint m_VerticesPerShape;

    std::vector<glm::vec4> m_Translations;

    void glSetup();
};

// void createParticle(uint programID, Particle particle);
// void updateParticles(uint programID);
// void drawParticles(uint programID);
// 
// Fun-ctions (:
// void createRandomParticles(uint programID, uint xAreaSize, uint yAreaSize, uint number, float minSpeed, float maxSpeed);

