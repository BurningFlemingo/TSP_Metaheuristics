#pragma once 

#include "../ICompute.h"
#include "pch.h"
#include "Utils.h"

class LorenzeAttractor : public ICompute {
public:
    LorenzeAttractor(const ComputeSettings& settings, const Shaders& shaders);
    ~LorenzeAttractor() {};

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

    uint m_BlurTexture;
    uint m_TrailTexture;

    SSBO m_SSBO_1;
    uint m_NumWorkGroups;
 };
