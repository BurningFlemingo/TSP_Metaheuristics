#pragma once 

#include "../ICompute.h"
#include "pch.h"
#include "Utils.h"

struct SlimeMoldSettings {
    uint count;
};

class SlimeMold : public ICompute {
public:
    SlimeMold(const ComputeSettings& settings, const Shaders& shaders, const SlimeMoldSettings& moldSettings);
    ~SlimeMold() {};

    virtual void setup() override;
    virtual void run(float dT) override;
private:
    struct Particle {
        glm::vec4 position;
        glm::vec4 color;
    };
    struct SSBO {
        std::vector<Particle> particles;
    };

    SlimeMoldSettings m_MoldSettings;

    uint m_BlurTexture;
    uint m_TrailTexture;

    SSBO m_SSBO_1;
    uint m_NumWorkGroups;

    void spawnParticles();
 };
