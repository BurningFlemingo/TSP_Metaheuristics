#include "SlimeMold.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"
#include "Renderer/Shader.h"
#include "Utils.h"

SlimeMold::SlimeMold(const ComputeSettings& settings, const Shaders& shaders, const SlimeMoldSettings& moldSettings)
: ICompute(settings, shaders), m_MoldSettings(moldSettings) {
}

void SlimeMold::setup() {
    spawnParticles();

    createComputeShader("../res/Shaders/SlimeMold.comp", 16);
    createComputeShader("../res/Shaders/Trail.comp", 8);

    createTexture(m_WindowTexture, 0, m_Settings.windowDimensions);
    createTexture(m_TrailTexture, 1, m_Settings.windowDimensions);

    createSSBO(0, 1, m_SSBO_1.particles.size() * (sizeof(Particle)), m_SSBO_1.particles.data());
    print(sizeof(Particle) / 4);
    
    m_MaxDeltaTime = 100;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void SlimeMold::spawnParticles() {
    for (int i = 0; i < (int)m_MoldSettings.count; i++) {
        float rad = (randNum(0, 360) * glm::pi<float>()) / 180;

        Particle temp;
        temp.position = glm::vec4(m_Settings.windowDimensions.x / 2, m_Settings.windowDimensions.y / 2, 0.0, rad);
        temp.color = glm::vec4(1.0f);

        m_SSBO_1.particles.emplace_back(temp);
    }
}

void SlimeMold::run(float dT) {
    print("delta time: " + std::to_string(dT));

    glUseProgram(m_ComputePrograms[0].first);
    glClearTexImage(m_WindowTexture, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_ComputePrograms[0].first);

    glUniform1ui(glGetUniformLocation(m_ComputePrograms[0].first, "time"), SDL_GetTicks());
    glUniform1ui(glGetUniformLocation(m_ComputePrograms[0].first, "count"), m_SSBOs[0].ssboSize[1]);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[0].first, "deltaTime"), dT);

    glBindTextureUnit(0, m_WindowTexture);
    glBindTextureUnit(1, m_TrailTexture);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    uint numWorkGroups = std::ceil((m_SSBOs[0].ssboSize[1] + m_ComputePrograms[0].second - 1) / m_ComputePrograms[0].second) * std::ceil(1 / m_Settings.scale);
    glDispatchCompute(numWorkGroups, 1, 1);

    glUseProgram(m_ComputePrograms[1].first);

    glUniform1f(glGetUniformLocation(m_ComputePrograms[1].first, "deltaTime"), dT);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[1].first, "decay"), m_Settings.decay);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[1].first, "diffusionRate"), m_Settings.diffusionRate);

    glBindTextureUnit(1, m_TrailTexture);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glDispatchCompute(m_Settings.windowDimensions.x / 8, m_Settings.windowDimensions.y / 8, 1);

    glUseProgram(m_WindowProgramID);
    glBindTextureUnit(0, m_TrailTexture);

    glUniform1i(glGetUniformLocation(m_WindowProgramID, "window"), 0);
    glUniform1f(glGetUniformLocation(m_WindowProgramID, "scale"), m_Settings.scale);

    glBindVertexArray(VAO);
    glCheckError(glDrawElements(GL_TRIANGLES, 6 * sizeof(uint), GL_UNSIGNED_INT, 0));
}
