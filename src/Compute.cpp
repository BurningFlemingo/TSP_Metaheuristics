#include "Compute.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"
#include "Renderer/Shader.h"
#include "Utils.h"

Compute::Compute(const ComputeSettings& settings)
: m_ComputeProgramID(0), m_Settings(settings){
    for (int i = -2500; i < 5001; i++) {
        Particle temp;
        temp.position = glm::vec3(i + 0.01, 0.00, 0);
        temp.rotation = ((float)randNum(0, 360) * glm::pi<float>()) / 180;

        m_Particles.emplace_back(temp);
    }
}

void Compute::glSetup() {
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

    glCreateTextures(GL_TEXTURE_2D, 1, &m_WindowTexture);
    glTextureParameteri(m_WindowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_WindowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_WindowTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_WindowTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(m_WindowTexture, 1, GL_RGBA32F, m_Settings.windowDimensions.x, m_Settings.windowDimensions.y);
    glBindImageTexture(0, m_WindowTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    m_WindowProgramID = createShaders({ "../res/Shaders/Window.frag", "../res/Shaders/Window.vert" }, { GL_FRAGMENT_SHADER, GL_VERTEX_SHADER });
    m_ComputeProgramID = createShaders({ "../res/Shaders/ComputePractice.comp" }, { GL_COMPUTE_SHADER });

    if (glGetError() != GL_NO_ERROR) {
        throw(0);
    }

    glGenBuffers(1, &SSBO_1);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_1);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_Particles.size() * sizeof(Particle), m_Particles.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO_1);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Compute::writeSSBO(std::vector<Particle>& particleVector) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_1);
    GLvoid* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, m_Particles.data(), m_Particles.size() * sizeof(Particle));

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void Compute::readSSBO(std::vector<Particle>& particleVector) {
    glGetNamedBufferSubData(SSBO_1, 0, m_Particles.size() * sizeof(Particle), m_Particles.data());
}

void Compute::draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glUseProgram(m_ComputeProgramID);
    glClearTexImage(m_WindowTexture, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_ComputeProgramID);

    glUniform1ui(glGetUniformLocation(m_ComputeProgramID, "time"), SDL_GetTicks());
    glUniform1ui(glGetUniformLocation(m_ComputeProgramID, "count"), m_Particles.size());

    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    uint numWorkGroups = std::ceil((m_Particles.size() + m_Settings.workGoupSize - 1) / m_Settings.workGoupSize) * std::ceil(1 / m_Settings.scale);
    glDispatchCompute(numWorkGroups, 1, 1);

    glUseProgram(m_WindowProgramID);
    glBindTextureUnit(0, m_WindowTexture);
    glUniform1i(glGetUniformLocation(m_WindowProgramID, "window"), 0);
    glUniform1f(glGetUniformLocation(m_WindowProgramID, "scale"), m_Settings.scale);

    glBindVertexArray(VAO);

    glCheckError(glDrawElements(GL_TRIANGLES, 6 * sizeof(uint), GL_UNSIGNED_INT, 0));

    std::cout << "(" << m_Particles[0].position.x << ", " << m_Particles[0].position.y << ", " << m_Particles[0].position.z << ")" << std::endl;
}
