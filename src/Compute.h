#pragma once 

#include "pch.h"

struct Particle {
    glm::vec3 position;
    float rotation;
};

struct ComputeSettings {
    glm::vec2 windowDimensions;
    float scale;

    uint workGoupSize;
};

class Compute {
public:
    Compute(const ComputeSettings& settings);
    void glSetup();

    void draw();
    void writeSSBO(std::vector<Particle>& particleVector);
    void readSSBO(std::vector<Particle>& particleVector);
private:
    ComputeSettings m_Settings;
    
    uint m_ComputeProgramID;
    uint m_WindowProgramID;
    uint VAO;
    uint SSBO_1;
    uint m_WindowTexture;
    uint m_WorkGroupSize;

    std::vector<Particle> m_Particles;
};
