#include "SlimeMold.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"
#include "Renderer/Shader.h"
#include "Utils.h"

SlimeMold::SlimeMold(const ComputeSettings& settings, const Shaders& shaders, std::shared_ptr<TSP> tsp, const SlimeMoldSettings& moldSettings)
: ICompute(settings, shaders, tsp), m_MoldSettings(moldSettings) {
}

SlimeMold::~SlimeMold() {

    glDeleteTextures(m_Textures.size(), m_Textures.data());
}

void SlimeMold::setup() {
    spawnParticles();

    createComputeShader("../res/Shaders/SlimeMold.comp", 16);
    createTexture(1, m_Settings.windowDimensions);

    createSSBO(2, 3, m_MoldSettings.count * (sizeof(Particle)), m_SSBO_1.particles.data());
    
    m_MaxDeltaTime = 1000;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // print("number of particles: " + std::to_string(m_SSBO_1.particles.size()));
}

void SlimeMold::spawnParticles() {
    m_Clock.startTimer();
    m_SSBO_1.particles.reserve(m_MoldSettings.count * sizeof(Particle));
    glm::vec2 dims = m_Settings.windowDimensions;
    for (int i = 0; i < (int)m_MoldSettings.count; i++) {
        float radius = 100;
        float rad = (randNum(0, 360) * glm::pi<float>()) / 180;
        glm::vec2 rotation = glm::vec2(cos(rad), sin(rad));

        Particle temp;
        temp.position = glm::vec4(m_TSP->getCitys()[0], 0.0, rad);

        m_SSBO_1.particles.push_back(temp);
    }
    // print("Particle spawn time: " + std::to_string((float)m_Clock.endTimer() / 1000000.0));
}

void SlimeMold::update(float dT) {
    glUseProgram(m_ComputePrograms[2].first);
    glClearTexImage(m_Textures[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_ComputePrograms[2].first);
    glUniform1ui(glGetUniformLocation(m_ComputePrograms[2].first, "time"), m_Clock.getTime());
    glUniform1ui(glGetUniformLocation(m_ComputePrograms[2].first, "count"), m_SSBO_1.particles.size());
    glUniform1f(glGetUniformLocation(m_ComputePrograms[2].first, "deltaTime"), dT);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[2].first, "detectionThreshold"), m_MoldSettings.detectionThreshold);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[2].first, "trailReleseThreshold"), m_MoldSettings.trailReleseThreshold);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[2].first, "turnThreshold"), m_MoldSettings.turnThreshold);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[2].first, "forwardRayDistance"), m_MoldSettings.forwardRayDistance);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[2].first, "sideRayDistances"), m_MoldSettings.sideRayDistances);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[2].first, "foodValue"), m_MoldSettings.foodValue);
    glUniform3f(glGetUniformLocation(m_ComputePrograms[2].first, "color"), m_MoldSettings.color.x, m_MoldSettings.color.y, m_MoldSettings.color.z);
    glBindTextureUnit(1, m_Textures[1]);
    glBindTextureUnit(2, m_TSP->getCityTexture());

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    uint numWorkGroups = std::ceil((m_SSBO_1.particles.size() + m_ComputePrograms[2].second - 1) / m_ComputePrograms[2].second) * std::ceil(1 / m_Settings.scale);
    glDispatchCompute(numWorkGroups, 1, 1);

    updateTrail(dT);
    updateTrail(dT);

    updatePaths(dT);
}

void SlimeMold::render() {
    renderTrail();
    renderPaths();
}

Data SlimeMold::exportData() {
    Data data;
    data.title = m_MoldSettings.title;
    data.sizeofPath = m_TSP->getDistanceOfPath(m_CurrentBestPath.path);
    return data;
}
