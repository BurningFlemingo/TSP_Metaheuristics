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
#include "ComputeShaders/LorenzAttractor.h"

class Application {
public:
    Application(const WindowPropertys& windowProps);
    ~Application();

    void renderInit();

    void mainLoop();
private:
    Window m_Window;
    WindowPropertys m_WindowProps;
    AppState m_AppState;
    EventStack m_Stack;

    uint m_ProgramID;
    std::vector<Metaheuristic*> m_Metaheuristics;

    std::vector<std::unique_ptr<ICompute>> m_ComputeShaders;

    ChronoClock<std::chrono::microseconds> m_Clock;
    float m_DeltaTime;
    uint32_t m_LastTime;
    float m_MaxDeltaTime;
    float m_MinDeltaTime;

    void deltaUpdate();
    void render();

    uint32_t m_Frames;
};
