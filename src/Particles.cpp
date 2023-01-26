#include "Particles.h"
#include "Utils.h"
#include "Core/Debug.h"
#include "Renderer/Shader.h"

// std::vector<Particle> particles;
// std::vector<uint> particleVAOs;

ParticleSystem::ParticleSystem(AppState& appState)
: m_AppState(appState), m_ParticleVAO(0), m_ParticleSize(2) {
    m_ProgramID = setupShaders("../res/Shaders/ParticleVertexShader.glsl", "../res/Shaders/ParticleFragmentShader.glsl");
}

void ParticleSystem::glSetup() {
    glUseProgram(m_ProgramID);
    setUniform4m(m_ProgramID, "projection", getProjectionMatrix());

    glGenVertexArrays(1, &m_ParticleVAO);
    glCheckError(glBindVertexArray(m_ParticleVAO));

    const float particleSize = (float)m_ParticleSize;

    float vertices[] = {
        0.0, 0.0,  // bottom left
        (float)(1.0 * particleSize), 0.0,  // bottom right
        0.0, (float)(1.0 * particleSize),  // top left
        (float)(1.0 * particleSize), (float)(1.0 * particleSize)  // top right
    };

    uint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_InstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, m_Translations.size() * sizeof(glm::vec2), &m_Translations[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);

    setUniform4m(m_ProgramID, "projection", getProjectionMatrix());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::createParticle(Particle particle) {
    m_Particles.emplace_back(particle);
}

void ParticleSystem::createRandomParticles(glm::vec2 areaSize, uint ammount, float minSpeed, float maxSpeed, uint particleSize) {
    m_ParticleSize = particleSize;
    for (int i = 0; i < ammount; i++) {
        int negativeDirX = 1 + randNum(0, 1) * -2;
        int negativeDirY = 1 + randNum(0, 1) * -2;
        Particle particle;
        particle.position.x = randNum(0, areaSize.x);
        particle.position.y = randNum(0, areaSize.y);
        particle.speed = randNum((int)minSpeed, (int)maxSpeed);
        particle.decay = (float)randNum(0, 1000) / 1000;
        particle.direction.x = ((float)randNum(0, 100) / 100) * negativeDirX;
        particle.direction.y = ((float)randNum(0, 100) / 100) * negativeDirY;

        createParticle(particle);
    }
}

void ParticleSystem::update(float dT) {
    m_Translations.clear();
    for (int i = 0; i < m_Particles.size(); i++) {
        Particle& particle = m_Particles[i];
        particle.position += particle.direction * particle.speed;
        particle.previousPositions.push(particle.position);
        for (int i = 0; i < particle.previousPositions.sizeUsed(); i++) {
            m_Translations.emplace_back(glm::vec3(particle.previousPositions[i], 1.0 / (i * 2) + 0.01));
        }
    }
}

void ParticleSystem::draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glCheckError(glUseProgram(m_ProgramID));
    glCheckError(glBindVertexArray(m_ParticleVAO));

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, m_Translations.size() * sizeof(glm::vec2), &m_Translations[0], GL_DYNAMIC_DRAW);

    glCheckError(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_Translations.size()));
    glBindVertexArray(0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
