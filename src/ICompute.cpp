#include "ICompute.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"
#include "Renderer/Shader.h"
#include "Utils.h"

ICompute::ICompute(const ComputeSettings& settings, const Shaders& shader, std::shared_ptr<TSP> tsp)
    : m_Settings(settings), m_TSP(tsp), m_MaxDeltaTime(3), m_MinDeltaTime(0.0), m_Initialized(false) {
    glSetup(shader);
    initTrailShader();
    initPathShader();

    m_Initialized = true;
}

ICompute::~ICompute() {
    glDeleteTextures(m_Textures.size(), &m_Textures[0]);

    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);

    glDeleteVertexArrays(1, &m_VAO);

    for (auto& buffer : m_SSBOInfo) {
        for (int i = 0; i < buffer.ssboID.size(); i++) {
            glDeleteBuffers(1, &buffer.ssboID[i]);
        }
    }

    for (auto& program : m_ComputePrograms) {
        glDeleteProgram(program.first);
    }
    glDeleteProgram(m_WindowProgramID);
}

void ICompute::glSetup(const Shaders& shader) {
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,   0.0f, 1.0f,  // bottom left
        -1.0f, 1.0f, 0.0f,    0.0f, 0.0f,  // top left 
        1.0f, 1.0f, 0.0f,     1.0f, 0.0f,  // top right 
        1.0f, -1.0f, 0.0f,    1.0f, 1.0f,  // bottom right 
    };
    uint indices[] {
        0, 2, 1,
        0, 3, 2
    };

    glCreateVertexArrays(1, &m_VAO);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferData(m_VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glCreateBuffers(1, &m_EBO);
    glNamedBufferData(m_EBO, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(m_VAO, 0);
    glVertexArrayAttribBinding(m_VAO, 0, 0);
    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexArrayAttrib(m_VAO, 1);
    glVertexArrayAttribBinding(m_VAO, 1, 0);
    glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

    glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, 5 * sizeof(float));
    glVertexArrayElementBuffer(m_VAO, m_EBO);

    m_WindowProgramID = createShaders({ "../res/Shaders/Window.frag", "../res/Shaders/Window.vert" }, { GL_FRAGMENT_SHADER, GL_VERTEX_SHADER });

    m_Textures.reserve(100 * sizeof(uint));
    for (int i = 0; i < 100; i++) {
        m_Textures.emplace_back(-1);
    }
}

void ICompute::initTrailShader() {
    createComputeShader("../res/Shaders/Trail.comp", 8);
    createTexture(0, m_Settings.windowDimensions);
}

void ICompute::initPathShader() {
    createComputeShader("../res/Shaders/GetCurrentPath.comp", 20);

    std::vector<glm::vec2> citys = m_TSP->getCitys();
    for(int i = 0; i < citys.size(); i++) {
        m_CityPositions.emplace_back(glm::vec4(citys[i].x, citys[i].y, 0, 0));
        m_CurrentPath.emplace_back(i);
    }
    m_CurrentBestPath.path = m_CurrentPath;
    m_CurrentBestPath.color = { 0, 255, 0, 255 };

    createSSBO(1, 1,  m_TSP->getCitys().size() * (sizeof(glm::vec4)), m_CityPositions.data());
    createSSBO(1, 2,  m_TSP->getCitys().size() * (sizeof(uint)), m_CurrentPath.data());

    writeSSBO(1, 1);
}

void ICompute::createSSBO(uint computeShaderIndex, uint binding, uint size, void* data) {
    SSBOInfo& info = m_SSBOInfo[computeShaderIndex];
    uint shaderID;
    glGenBuffers(1, &shaderID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, shaderID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, shaderID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    info.ssboID.insert({ binding, shaderID });
    info.ssboSize.insert({ binding, size });
    info.ssboData.insert({ binding, data });
    info.IDs.emplace_back(shaderID);
}

void ICompute::createTexture(uint binding, const glm::vec2& size) {
    uint& textureID = m_Textures[binding];
    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(textureID, 1, GL_RGBA32F, size.x, size.y);
    glBindImageTexture(binding, textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}


void ICompute::createComputeShader(std::string shaderPath, uint numWorkGroups) {
    std::pair<uint, uint> pair(createShaders({ shaderPath }, { GL_COMPUTE_SHADER }), numWorkGroups);
    m_ComputePrograms.emplace_back(pair);
    SSBOInfo temp;
    m_SSBOInfo.emplace_back(temp);  // index is the associated compute shader
}

void ICompute::writeSSBO(uint computeShaderIndex, uint binding) {
    SSBOInfo& info = m_SSBOInfo[computeShaderIndex];
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, info.ssboID[binding]);
    GLvoid* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, info.ssboData[binding], info.ssboSize[binding]);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void ICompute::readSSBO(uint computeShaderIndex, uint binding) {
    SSBOInfo& info = m_SSBOInfo[computeShaderIndex];
    glGetNamedBufferSubData(info.ssboID[binding], 0, info.ssboSize[binding], info.ssboData[binding]);
}

void ICompute::updateTrail(float dT) {
    glUseProgram(m_ComputePrograms[0].first);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[0].first, "deltaTime"), dT);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[0].first, "decay"), m_Settings.decay);
    glUniform1f(glGetUniformLocation(m_ComputePrograms[0].first, "diffusionRate"), m_Settings.diffusionRate);
    bindTrail();

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glDispatchCompute(std::ceil(m_Settings.windowDimensions.x / 8), std::ceil(m_Settings.windowDimensions.y / 8), 1);
}

void ICompute::updatePaths(float dT) {
    glUseProgram(m_ComputePrograms[1].first);
    glUniform1ui(glGetUniformLocation(m_ComputePrograms[1].first, "count"), m_TSP->getCitys().size());
    glUniform1ui(glGetUniformLocation(m_ComputePrograms[1].first, "time"), SDL_GetTicks());
    bindTrail();

    for (int i = 0; i < m_CurrentPath.size(); i++) {
        m_CurrentPath[i] = -1;
    }
    writeSSBO(1, 2);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glDispatchCompute(std::ceil(m_TSP->getCitys().size() / 64.0), 1, 1);

    readSSBO(1, 2);
    if (m_TSP->getDistanceOfPath(m_CurrentBestPath.path) > m_TSP->getDistanceOfPath(m_CurrentPath)) {
        m_CurrentBestPath.path = m_CurrentPath;
    }
}

void ICompute::renderTrail() {
    glUseProgram(m_WindowProgramID);
    bindTrail();

    glUniform1i(glGetUniformLocation(m_WindowProgramID, "window"), 0);
    glUniform1f(glGetUniformLocation(m_WindowProgramID, "scale"), m_Settings.scale);

    glBindVertexArray(m_VAO);
    glCheckError(glDrawElements(GL_TRIANGLES, 6 * sizeof(uint), GL_UNSIGNED_INT, 0));
}

void ICompute::renderPaths() {
    TSPath currentPath;
    TSPath bestPath;

    currentPath.path = m_CurrentPath;
    currentPath.color = { 255, 255, 255, 40 };
    bestPath.path = m_TSP->getBestPath();
    bestPath.color = { 0, 0, 255, 50 };

    m_CurrentBestPath.color = { 0, 255, 0, 255 };
    m_TSP->renderPath(currentPath);
    m_TSP->renderPath(m_CurrentBestPath);
    m_TSP->renderPath(bestPath);
}
