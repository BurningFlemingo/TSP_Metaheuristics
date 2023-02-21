#include "TSP.h"
#include "Utils.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Core/Debug.h"

TSP::TSP(const TSPSettings& settings)
: m_Settings(settings), m_CityVAO(0), m_PathVAO(0), m_Program(0), m_CityVBO(0), m_PathVBO(0), m_CityTexture(0) {
    m_Citys.reserve(m_Settings.numberOfCitys * sizeof(glm::vec2));
    m_Nodes.reserve(m_Settings.numberOfCitys * sizeof(glm::vec2) * settings.cityResolution);

    generateCitys();
    findBestPath();
    glSetup();
}

TSP::~TSP() {
    glDeleteProgram(m_Program);
    glDeleteTextures(1, &m_CityTexture);
    glDeleteVertexArrays(1, &m_CityVAO);
    glDeleteVertexArrays(1, &m_PathVAO);

    glDeleteBuffers(1, &m_CityVBO);
    glDeleteBuffers(1, &m_PathVBO);
}

void TSP::glSetup() {
    m_Program = createShaders( { "../res/Shaders/TSP.vert", "../res/Shaders/TSP.frag" }, { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER } );

    glCreateVertexArrays(1, &m_CityVAO);
    glCreateBuffers(1, &m_CityVBO);

    glNamedBufferData(m_CityVBO, m_Settings.numberOfCitys * m_Settings.cityResolution * sizeof(glm::vec2), m_Nodes.data(), GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(m_CityVAO, 0);
    glVertexArrayAttribBinding(m_CityVAO, 0, 0);
    glVertexArrayAttribFormat(m_CityVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

    glVertexArrayVertexBuffer(m_CityVAO, 0, m_CityVBO, 0, sizeof(glm::vec2));

    glCreateVertexArrays(1, &m_PathVAO);
    glCreateBuffers(1, &m_PathVBO);

    glEnableVertexArrayAttrib(m_PathVAO, 0);
    glVertexArrayAttribBinding(m_PathVAO, 0, 0);
    glVertexArrayAttribFormat(m_PathVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

    glVertexArrayVertexBuffer(m_PathVAO, 0, m_PathVBO, 0, sizeof(glm::vec2));

    glUseProgram(m_Program);
    glUniformMatrix4fv(glGetUniformLocation(m_Program, "projection"), 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));

    glLineWidth(m_Settings.lineWidth);
}

glm::vec2 TSP::generatePoint() {
    glm::vec2 point = glm::vec2(randNum(0, m_Settings.spawnArea.x), randNum(0, m_Settings.spawnArea.y));
    return point;
}

void TSP::generateCitys() {
    for (int i = 0; i < m_Settings.numberOfCitys; i++) {
        m_Citys.emplace_back(generatePoint());
    }
    citysToCircles();
}

void TSP::generateCityTexture(uint binding) {
    uint FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    uint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(texture, 1, GL_RGBA32F, m_Settings.spawnArea.x, m_Settings.spawnArea.y);
    glBindImageTexture(binding, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glUseProgram(m_Program);
    glBindVertexArray(m_CityVAO);

    glm::mat4 projection = glm::ortho(0.0f, m_Settings.spawnArea.x, 0.0f, m_Settings.spawnArea.y, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glm::vec4 color = m_Settings.cityColor;
    glUniform4f(glGetUniformLocation(m_Program, "color"), color.x / 255, color.y / 255, color.z / 255, color.w / 255);
    for (int i = 0; i < m_Citys.size(); i++) {
        glCheckError(glDrawArrays(GL_TRIANGLE_FAN, i * m_Settings.cityResolution, m_Settings.cityResolution));
    }

    glUniformMatrix4fv(glGetUniformLocation(m_Program, "projection"), 1, GL_FALSE, glm::value_ptr(getProjectionMatrix()));

    m_CityTexture = texture;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &FBO);
}

void TSP::citysToCircles() {
    for (int i = 0; i < m_Citys.size(); i++) {
        float angleIncrement = 360.0 / m_Settings.cityResolution;
        for (int j = 0; j < m_Settings.cityResolution; j++) {
            float rad = ((angleIncrement * (j + 1)) * glm::pi<float>()) / 180;
            glm::vec2 city = glm::vec2(m_Citys[i].x, m_Citys[i].y);
            glm::vec2 pos = city + glm::vec2(cos(rad), sin(rad)) * m_Settings.cityRadius;
            m_Nodes.push_back(pos);
        }
    }
}

 void TSP::pushPath(const TSPath& path) {
    assert(path.path.size() == m_Citys.size());
    m_Paths.emplace_back(path);

    std::vector<glm::vec2> pathway;
    for (int i = 0; i < m_Citys.size(); i++) {
        pathway.emplace_back(m_Citys[path.path[i]]);
    }
    m_PathPositions.emplace_back(pathway);
}

void TSP::popPath() {
    m_Paths.pop_back();
    m_PathPositions.pop_back();
}

void TSP::clearPaths() {
    m_Paths.clear();
    m_PathPositions.clear();
}

float TSP::getDistanceOfPath(const std::vector<uint>& path) {
    assert(path.size() == m_Citys.size());

    float distance = 0;
    for (int i = 0; i < m_Citys.size() - 1; i++) {
        glm::vec2 p1 = m_Citys[path[i]];
        glm::vec2 p2 = m_Citys[path[i + 1]];
        glm::vec2 dXY = p2 - p1;
        float displacement = sqrt(dXY.x * dXY.x + dXY.y * dXY.y);
        distance += displacement;
    }
    return distance;
}

void TSP::findBestPath() {
    std::vector<uint> citys;
    std::vector<uint> bestCity;
    for (int i = 0; i < m_Citys.size(); i++) {
        citys.emplace_back(i);
        bestCity.emplace_back(i);
    }

    uint shortestPath = UINT_MAX;
    do {
        uint currentPathWeight = 0;

        currentPathWeight = getDistanceOfPath(citys);

        shortestPath = std::min(shortestPath, currentPathWeight);
        if (shortestPath == currentPathWeight) {
            bestCity = citys;
        }
    } while (
        std::next_permutation(citys.begin(), citys.end())
    );

    m_BestPath = bestCity;
    m_SizeOfBestPath = getDistanceOfPath(bestCity);
}

void TSP::renderCitys() {
    glUseProgram(m_Program);
    glBindVertexArray(m_CityVAO);

    glm::vec4 color = m_Settings.cityColor;
    glUniform4f(glGetUniformLocation(m_Program, "color"), color.x / 255, color.y / 255, color.z / 255, color.w / 255);
    for (int i = 0; i < m_Citys.size(); i++) {
        glCheckError(glDrawArrays(GL_TRIANGLE_FAN, i * m_Settings.cityResolution, m_Settings.cityResolution));
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void TSP::renderPaths() {
    glUseProgram(m_Program);
    glBindVertexArray(m_PathVAO);

    glBindVertexArray(m_PathVAO);
    glUseProgram(m_Program);

    for (int i = 0; i < m_Paths.size(); i++) {

        TSPath path = m_Paths[i];

        glNamedBufferData(m_PathVBO, m_Citys.size() * sizeof(glm::vec2), m_PathPositions[i].data(), GL_STATIC_DRAW);
        glUniform4f(glGetUniformLocation(m_Program, "color"), path.color.x / 255, path.color.y / 255, path.color.z / 255, path.color.w / 255);
        glCheckError(glDrawArrays(GL_LINE_LOOP, 0, m_Settings.numberOfCitys));
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void TSP::renderPath(const TSPath& path) {
    std::vector<glm::vec2> pathway;
    for (int i = 0; i < m_Citys.size(); i++) {
        pathway.emplace_back(m_Citys[path.path[i]]);
    }

    glBindVertexArray(m_PathVAO);

    glNamedBufferData(m_PathVBO, m_Citys.size() * sizeof(glm::vec2), pathway.data(), GL_STATIC_DRAW);
    glUseProgram(m_Program);

    glUniform4f(glGetUniformLocation(m_Program, "color"), path.color.x / 255, path.color.y / 255, path.color.z / 255, path.color.w / 255);
    glCheckError(glDrawArrays(GL_LINE_LOOP, 0, m_Settings.numberOfCitys));

    glBindVertexArray(0);
    glUseProgram(0);
}
