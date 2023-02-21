#pragma once 

#include "pch.h"
#include "Utils.h"
#include "TSP.h"

struct Data {
    std::string title;

    uint sizeofPath;
    std::vector<uint> other;
};

struct ComputeSettings {
    glm::vec2 windowDimensions;
    float scale;
    float decay;
    float diffusionRate;

    uint workGoupSize;
};

struct Shaders {
    std::string vertexShader;
    std::string fragmentShader;
};

class ICompute {
public:
    ICompute(const ComputeSettings& settings, const Shaders& shaders, std::shared_ptr<TSP> tsp);
    virtual ~ICompute();

    inline TSPath getCurrentBestPath() { return m_CurrentBestPath; }
    inline float getCurrentBestPathDistance() { return m_TSP->getDistanceOfPath(m_CurrentBestPath.path); }

    virtual void setup() = 0;
    virtual void update(float dT) = 0;
    virtual void render() = 0;

    virtual Data exportData() = 0;
protected:
    void initTrailShader();
    void initPathShader();

    void updateTrail(float dT);
    void updatePaths(float dT);

    void renderTrail();
    void renderPaths();

    inline void bindTrail() { glBindTextureUnit(0, m_Textures[0]); }

    struct SSBOInfo {
        std::unordered_map<uint, uint> ssboID;  // binding to shaderID
        std::unordered_map<uint, uint> ssboSize;  // binding to size
        std::unordered_map<uint, void*> ssboData;  // binding to data pointer
        std::vector<uint> IDs;  // used to get all SSBOs
    };

    uint m_VAO;
    uint m_VBO;
    uint m_EBO;

    Data m_Data;

    ComputeSettings m_Settings;

    uint m_MaxDeltaTime;
    uint m_MinDeltaTime;

    std::vector<std::pair<uint, uint>> m_ComputePrograms;  // ID, number of work groups
    
    uint m_WindowProgramID;

    std::vector<uint> m_Textures;  // binding to textureID

    std::vector<SSBOInfo> m_SSBOInfo;  // index is the compute shader thats its binded to

    void createTexture(uint binding, const glm::vec2& size);
    void createSSBO(uint computeShaderIndex, uint binding, uint size, void* data);
    void createComputeShader(std::string shaderPath, uint numWorkGroups);

    void writeSSBO(uint computeShaderIndex, uint binding);
    void readSSBO(uint computeShaderIndex, uint binding);

    ChronoClock<std::chrono::microseconds> m_Clock;

    std::shared_ptr<TSP> m_TSP;
    TSPath m_CurrentBestPath;

    std::vector<uint> m_CurrentPath;
    std::vector<glm::vec4> m_CityPositions;

private:
    void glSetup(const Shaders& shaders);

    bool m_Initialized;
 };
