#include "Particles.h"
#include "Utils.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"

// std::vector<Particle> particles;
// std::vector<uint> particleVAOs;

ParticleSystem::ParticleSystem(AppState& appState)
: m_AppState(appState), m_ParticleVAO(0) {
    m_ProgramID = setupShaders("../res/Shaders/VertexShader.glsl", "../res/Shaders/FragmentShader.glsl");
    glSetup();
}

void ParticleSystem::glSetup() {
    glUseProgram(m_ProgramID);
    setUniform4m(m_ProgramID, "projection", getProjectionMatrix());

    glGenVertexArrays(1, &m_ParticleVAO);
    glCheckError(glBindVertexArray(m_ParticleVAO));

    float vertices[] = {
        0.0, 0.0,  // bottom left
        1.0, 0.0,  // bottom right
        0.0, 1.0,  // top left
        1.0, 1.0  // top right
    };

    uint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    VertexBufferLayout layout;
    addVertexBufferElement(layout, GL_FLOAT, 2);
    addVertexAttributes(layout);

    glm::mat4 model = glm::mat4(1.0f);
    setUniform4m(m_ProgramID, "model", model);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::createParticle(Particle particle) {
    m_Particles.emplace_back(particle);
}

void ParticleSystem::createRandomParticles(glm::vec2 areaSize, uint ammount, float minSpeed, float maxSpeed) {
    for (int i = 0; i < ammount; i++) {
        Particle particle;
        particle.quad.origin.x = randNum(0, areaSize.x);
        particle.quad.origin.y = randNum(0, areaSize.y);
        particle.quad.w = 2;
        particle.quad.h = 2;
        particle.speed = randNum((int)minSpeed, (int)maxSpeed);
        particle.decay = (float)randNum(0, 1000) / 1000;
        particle.direction.x = (float)randNum(0, 100) / 100;
        particle.direction.y = (float)randNum(0, 100) / 100;

        createParticle(particle);
    }
}

void ParticleSystem::update(float dT) {
    for (auto& particle : m_Particles) {
        particle.quad.origin += particle.direction * particle.speed;
    }
}

void ParticleSystem::draw() {
    glCheckError(glUseProgram(m_ProgramID));
    glCheckError(glBindVertexArray(m_ParticleVAO));

    for (int i = 0; i < m_Particles.size(); i++) {
        Quad& quad = m_Particles[i].quad;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(quad.origin.x, quad.origin.y, 0.0f));  
        model = glm::scale(model, glm::vec3(quad.w, quad.h, 1.0f));

        setUniform4m(m_ProgramID, "model", model);
 
        glCheckError(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    }
    glBindVertexArray(0);
}

// void createParticle(uint programID, Particle particle) {
//     particleVAOs.emplace_back(createQuad(programID, particle.quad, 0, 0, GL_DYNAMIC_DRAW));
//     particles.emplace_back(particle);
// }
// 
// void updateParticles(uint programID) {
//     for (auto& particle : particles) {
//         particle.quad.origin.x += particle.direction.x * particle.speed;
//         particle.quad.origin.y += particle.direction.y * particle.speed;
//         if (particle.speed > 0) {
//             particle.speed -= particle.decay;
//         }
//     }
// }
// 
// void drawParticles(uint programID) {
//     glBindVertexArray(particleVAOs[0]);
//     for (int i = 0; i < particleVAOs.size(); i++) {
//         Quad& quad = particles[i].quad;
// 
//         glm::mat4 model = glm::mat4(1.0f);
//         model = glm::translate(model, glm::vec3(quad.origin.x, quad.origin.y, 0.0f));  
//         model = glm::scale(model, glm::vec3(quad.w, quad.h, 1.0f));
// 
//         setUniform4m(programID, "model", model);
// 
//         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//     }
//     glBindVertexArray(0);
// }
// 
// void createRandomParticles(uint programID, uint xAreaSize, uint yAreaSize, uint number, float minSpeed, float maxSpeed) {
//     for (int i = 0; i < number; i++) {
//         Particle particle;
//         particle.quad.origin.x = randNum(0, xAreaSize);
//         particle.quad.origin.y = randNum(0, yAreaSize);
//         particle.quad.w = 2;
//         particle.quad.h = 2;
//         particle.speed = randNum((int)minSpeed, (int)maxSpeed);
//         particle.decay = (float)randNum(0, 1000) / 1000;
//         particle.direction.x = (float)randNum(0, 100) / 100;
//         particle.direction.y = (float)randNum(0, 100) / 100;
// 
//         createParticle(programID, particle);
//     }
//     
// }
