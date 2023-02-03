#include "ICompute.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"
#include "Renderer/Shader.h"
#include "Utils.h"

ICompute::ICompute(const ComputeSettings& settings, const Shaders& shader)
    : m_Settings(settings), m_MaxDeltaTime(3), m_MinDeltaTime(0.0) {
    glSetup(shader);
}

ICompute::~ICompute() {}

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

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    uint VBO;
    glCreateBuffers(1, &VBO);
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint EBO;
    glCreateBuffers(1, &EBO);
    glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));
    glVertexArrayElementBuffer(VAO, EBO);

    m_WindowProgramID = createShaders({ "../res/Shaders/Window.frag", "../res/Shaders/Window.vert" }, { GL_FRAGMENT_SHADER, GL_VERTEX_SHADER });
}

void ICompute::createSSBO(uint computeShaderIndex, uint binding, uint size, void* data) {
    SSBOInfo& info = m_SSBOs[computeShaderIndex];
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

void ICompute::createTexture(uint& textureID, uint binding, const glm::vec2& size) {
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
    m_SSBOs.emplace_back(temp);  // index is the associated compute shader
}

void ICompute::writeSSBO(uint computeShaderIndex, uint binding) {
    SSBOInfo& info = m_SSBOs[computeShaderIndex];
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, info.ssboID[binding]);
    GLvoid* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, info.ssboData[binding], info.ssboSize[binding]);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void ICompute::readSSBO(uint computeShaderIndex, uint binding) {
    SSBOInfo& info = m_SSBOs[computeShaderIndex];
    glGetNamedBufferSubData(info.ssboID[binding], binding, info.ssboSize[binding] - 1, info.ssboData[binding]);
}
