#pragma once 

#include "../ICompute.h"
#include "pch.h"
#include "Utils.h"
#include "../TSP.h"

struct SlimeMoldSettings {
    uint count;
    glm::vec3 color;
    float detectionThreshold;
    float trailReleseThreshold;
    float turnThreshold;
    float forwardRayDistance;
    float sideRayDistances;
    float foodValue;

    std::string title;
};

class SlimeMold : public ICompute {
public:
    SlimeMold(const ComputeSettings& settings, const Shaders& shaders, std::shared_ptr<TSP> tsp, const SlimeMoldSettings& moldSettings);
    ~SlimeMold();

    virtual void setup() override;
    virtual void update(float dT) override;
    virtual void render() override;

    virtual Data exportData() override;
private:

    struct Particle {
        glm::vec4 position;
    };
    struct SSBO {
        std::vector<Particle> particles;
    };

    SlimeMoldSettings m_MoldSettings;

    SSBO m_SSBO_1;
    std::vector<glm::vec4> m_CityPositions;
    std::vector<uint> m_CurrentPath;

    uint m_WindowTexture;

    uint m_NumWorkGroups;

    void spawnParticles();
    void getCurrentPath();
 };
