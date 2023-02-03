#include "LorenzAttractor.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"
#include "Renderer/Shader.h"
#include "Utils.h"

LorenzeAttractor::LorenzeAttractor(const ComputeSettings& settings, const Shaders& shaders)
: ICompute(settings, shaders) {
}

void LorenzeAttractor::setup() {
    m_Clock.startTimer();
    for (int i = -50000; i < 50001; i++) {
        uint radius = randNum(1, 20);
        // float rad = (randNum(0, 360) * glm::pi<float>()) / 180;
        glm::vec2 rad = glm::vec2(randNum(-180, 180), randNum(-180, 180));
        float mag = sqrt(rad.x * rad.x + rad.y * rad.y);
        glm::vec2 point = rad / mag;
        // glm::vec2 rotVec = glm::vec2(cos(rad) - sin(rad), sin(rad) + cos(rad));

        Particle temp;
        temp.position = glm::vec4(point.x * radius, point.y * radius, randNum(-10, 10), 1);
        temp.color = glm::vec4(1.0f);

        m_SSBO_1.particles.emplace_back(temp);
    }
    print(m_Clock.endTimer());
    createComputeShader("../res/Shaders/LorenzAttractor.comp", 16);
    createComputeShader("../res/Shaders/Trail.comp", 8);

    createTexture(m_WindowTexture, 0, m_Settings.windowDimensions);
    createTexture(m_TrailTexture, 1, m_Settings.windowDimensions);

    createSSBO(0, 1, m_SSBO_1.particles.size() * sizeof(Particle), m_SSBO_1.particles.data());
    
    m_MaxDeltaTime = 100;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void LorenzeAttractor::run(float dT) {
    print("delta time: " + std::to_string(dT));

    glUseProgram(m_ComputePrograms[0].first);
    glClearTexImage(m_WindowTexture, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_ComputePrograms[0].first);

    glBindTextureUnit(0, m_WindowTexture);
    glBindTextureUnit(1, m_TrailTexture);

    glUniform1ui(glGetUniformLocation(m_ComputePrograms[0].first, "time"), SDL_GetTicks());
    glUniform1ui(glGetUniformLocation(m_ComputePrograms[0].first, "count"), m_SSBOs[0].ssboSize[1]);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[0].first, "deltaTime"), dT);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    uint numWorkGroups = std::ceil((m_SSBOs[0].ssboSize[1] + m_ComputePrograms[0].second - 1) / m_ComputePrograms[0].second) * std::ceil(1 / m_Settings.scale);
    glDispatchCompute(numWorkGroups, 1, 1);

    readSSBO(0, 1);

    glUseProgram(m_ComputePrograms[1].first);
    glBindTextureUnit(1, m_TrailTexture);

    glUniform1f(glGetUniformLocation(m_ComputePrograms[1].first, "deltaTime"), dT);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[1].first, "decay"), 0.3);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[1].first, "diffuseSpeed"), 0.5);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glDispatchCompute(m_Settings.windowDimensions.x / 8, m_Settings.windowDimensions.y / 8, 1);

    glUseProgram(m_WindowProgramID);
    glBindTextureUnit(1, m_TrailTexture);

    glUniform1i(glGetUniformLocation(m_WindowProgramID, "window"), 1);
    glUniform1f(glGetUniformLocation(m_WindowProgramID, "scale"), m_Settings.scale);

    glBindVertexArray(VAO);
    glCheckError(glDrawElements(GL_TRIANGLES, 6 * sizeof(uint), GL_UNSIGNED_INT, 0));
}


