#include "AntColony.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"
#include "Renderer/Shader.h"
#include "Utils.h"

AntColony::AntColony(const ComputeSettings& settings, const Shaders& shaders, std::shared_ptr<TSP> tsp, const AntSettings& antSettings)
: ICompute(settings, shaders, tsp), m_AntSettings(antSettings) {
}

AntColony::~AntColony() {

    glDeleteTextures(m_Textures.size(), m_Textures.data());
}

void AntColony::setup() {
    spawnParticles();

    createComputeShader("../res/Shaders/AntColony.comp", 16);
    createTexture(1, m_Settings.windowDimensions);

    createSSBO(2, 3, m_AntSettings.count * (sizeof(Particle)), m_SSBO_1.particles.data());
    
    m_MaxDeltaTime = 1000;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void AntColony::spawnParticles() {
    m_Clock.startTimer();
    m_SSBO_1.particles.reserve(m_AntSettings.count * sizeof(Particle));
    glm::vec2 dims = m_Settings.windowDimensions;
    for (int i = 0; i < (int)m_AntSettings.count; i++) {
        float radius = 100;
        float rad = (randNum(0, 360) * glm::pi<float>()) / 180.0;
        glm::vec2 rotation = glm::vec2(cos(rad), sin(rad));

        Particle temp;
        temp.position = glm::vec4(m_TSP->getCitys()[0] + glm::vec2(100), 0.0, rad);
        temp.velocity = glm::vec4((float)rotation.x * 1000.0, (float)rotation.y * 1000.0, 0.0, 0.0);

        m_SSBO_1.particles.push_back(temp);
    }
}

void AntColony::update(float dT) {
    glUseProgram(m_ComputePrograms[2].first);
    glClearTexImage(m_Textures[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_ComputePrograms[2].first);
    glUniform1ui(glGetUniformLocation(m_ComputePrograms[2].first, "time"), m_Clock.getTime());
    glUniform1ui(glGetUniformLocation(m_ComputePrograms[2].first, "count"), m_SSBO_1.particles.size());
    glUniform1f(glGetUniformLocation(m_ComputePrograms[2].first, "deltaTime"), dT);
    glBindTextureUnit(1, m_Textures[1]);
    glBindTextureUnit(2, m_TSP->getCityTexture());

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    uint numWorkGroups = std::ceil((m_SSBO_1.particles.size() + m_ComputePrograms[2].second - 1) / m_ComputePrograms[2].second) * std::ceil(1 / m_Settings.scale);
    glDispatchCompute(numWorkGroups, 1, 1);

    readSSBO(2, 3);
    print(m_SSBO_1.particles[0].position.x);
    print(m_SSBO_1.particles[0].position.y);

    updateTrail(dT);
    updateTrail(dT);

    updatePaths(dT);
}

void AntColony::render() {
    renderTrail();
    renderPaths();
}

Data AntColony::exportData() {
    Data data;
    data.title = "Ant Colony Optimization";
    data.sizeofPath = m_TSP->getDistanceOfPath(m_CurrentBestPath.path);
    return data;
}
