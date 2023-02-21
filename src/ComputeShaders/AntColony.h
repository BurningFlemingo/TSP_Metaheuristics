#pragma once 

#include "../ICompute.h"
#include "pch.h"
#include "Utils.h"
#include "../TSP.h"

struct AntSettings {
    uint count;
    glm::vec3 color;
};

class AntColony : public ICompute {
public:
    AntColony(const ComputeSettings& settings, const Shaders& shaders, std::shared_ptr<TSP> tsp, const AntSettings& antColony);
    ~AntColony();

    virtual void setup() override;
    virtual void update(float dT) override;
    virtual void render() override;

    virtual Data exportData() override;
private:

    struct Particle {
        glm::vec4 position;
        glm::vec4 velocity;
    };
    struct SSBO {
        std::vector<Particle> particles;
    };

    AntSettings m_AntSettings;

    SSBO m_SSBO_1;
    std::vector<glm::vec4> m_CityPositions;
    std::vector<uint> m_CurrentPath;

    uint m_WindowTexture;

    uint m_NumWorkGroups;

    void spawnParticles();
    void getCurrentPath();
};
