#pragma once

#include "pch.h"
#include "Core/Debug.h"
#include "Core/Window.h"
#include "Core/EventHandler.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Metaheuristic.h"
#include "Utils.h"
#include "ICompute.h"
#include "TSP.h"

struct Parameters {
    uint numberOfCitys;
    uint ticksPerSimulation;

    float decay;
    float diffusion;
    float detectionThreshold;
    float trailReleseThreshold;
    float turnThreshold;
    float forwardRayDistance;
    float sideRayDistances;
    float foodValue;
    glm::vec3 color;

    uint numberOfSimulations;
    uint numberOfParticles;

    std::string title;
};

class Application {
public:
    Application(const WindowPropertys& windowProps);
    ~Application();

    void run();
private:
    bool runSimulation(const Parameters& parameter);
    bool runMetaheuristic(uint ticks);
    void graphData();

    bool m_SimulationsCompleted;

    Window m_Window;
    WindowPropertys m_WindowProps;
    bool m_Running;

    uint m_ProgramID;

    std::unique_ptr<ICompute> m_ComputeShader;
    std::shared_ptr<TSP> m_TSP;

    ChronoClock<std::chrono::microseconds> m_Clock;
    float m_DeltaTime;
    uint32_t m_LastTime;
    float m_MaxDeltaTime;
    float m_MinDeltaTime;

    void initTSP(uint numberOfCitys);
    void initRenderer();

    void spawnSMO(const Parameters& parameters);
    void spawnACO(uint count, float decay, float diffusion);

    void deltaUpdate();
    void pollEvents();
    void update();
    void render();

    void collectData();
    void setData();
    void printData();

    std::vector<std::vector<Data>> m_CollectedData;  // [trial][data]
    std::vector<Data> m_TrialData;

    uint32_t m_Frames;

    uint m_SimulationsRun;

    xlnt::workbook wb;
    xlnt::worksheet ws;

    bool paused;
};
