#pragma once

#include "pch.h"

struct TSPSettings {
    TSPSettings()
    : numberOfCitys(3), spawnArea({ 1920, 1080 }), lineWidth(2), cityRadius(5),
    cityResolution(20), cityColor({ 255, 255, 255, 255 }), bestPathColor(0, 255, 0, 255) {}
    uint numberOfCitys;
    glm::vec2 spawnArea;
    float lineWidth;
    float cityRadius;
    float cityResolution;
    glm::vec4 cityColor;
    glm::vec4 bestPathColor;
};

struct TSPath {
    std::vector<uint> path;
    glm::vec4 color;
};

class TSP {
public:
    TSP() = default;
    TSP(const TSPSettings& settings);
    ~TSP();

    void pushPath(const TSPath& path);
    void popPath();
    void removePath(uint index);
    void clearPaths();

    void renderCitys();

    void renderBestPath();
    void renderPaths();
    void renderPath(const TSPath& path);

    float getDistanceOfPath(const std::vector<uint>& path);

    inline std::vector<glm::vec2> getCitys() { return m_Citys; }
    inline TSPath getPath(uint index) const { return m_Paths[index]; }
    inline uint sizeofPaths() const { return m_Paths.size(); }

    void generateCityTexture(uint binding);
    inline uint getCityTexture() { return m_CityTexture; }
    inline uint getSizeOfBestPath() { return m_SizeOfBestPath; }
    inline std::vector<uint> getBestPath() { return m_BestPath; }
private:
    TSPSettings m_Settings;
    std::vector<glm::vec2> m_Citys;
    std::vector<TSPath> m_Paths;
    std::vector<std::vector<glm::vec2>> m_PathPositions;
    std::vector<glm::vec2> m_Nodes;  // m_Citys converted to a circle 

    std::vector<uint> m_BestPath;
    uint m_SizeOfBestPath;

    uint m_Program;
    uint m_EBO;
    uint m_CityVAO;
    uint m_PathVAO;
    uint m_CityVBO;
    uint m_PathVBO;

    uint m_CityTexture;

    void glSetup();

    void generateCitys();
    glm::vec2 generatePoint();
    void citysToCircles();

    void findBestPath();
};
