#pragma once 

#include "pch.h"
#include "Utils.h"

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
    ICompute(const ComputeSettings& settings, const Shaders& shaders);
    virtual ~ICompute() = 0;

    virtual void setup() = 0;
    virtual void run(float dT) = 0;
protected:
    struct SSBOInfo {
        std::unordered_map<uint, uint> ssboID;  // binding to shaderID
        std::unordered_map<uint, uint> ssboSize;  // binding to size
        std::unordered_map<uint, void*> ssboData;  // binding to data pointer
        std::vector<uint> IDs;  // used to get all SSBOs
    };

    ComputeSettings m_Settings;

    uint m_MaxDeltaTime;
    uint m_MinDeltaTime;

    std::vector<SSBOInfo> m_SSBOs;  // index is the compute shader thats its binded to
    std::vector<std::pair<uint, uint>> m_ComputePrograms;  // ID, number of work groups
    
    uint m_WindowProgramID;
    uint m_WindowTexture;
    uint VAO;

    void createTexture(uint& textureID, uint binding, const glm::vec2& size);
    void createSSBO(uint computeShaderIndex, uint binding, uint size, void* data);
    void createComputeShader(std::string shaderPath, uint numWorkGroups);

    void writeSSBO(uint computeShaderIndex, uint binding);
    void readSSBO(uint computeShaderIndex, uint binding);

    ChronoClock<std::chrono::microseconds> m_Clock;
private:
    void glSetup(const Shaders& shaders);
 };
