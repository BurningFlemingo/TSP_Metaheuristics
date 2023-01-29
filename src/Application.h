#pragma once

#include "pch.h"
#include "Core/Debug.h"
#include "Core/Window.h"
#include "Core/EventHandler.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Metaheuristic.h"
#include "Utils.h"

class Application {
public:
    Application(const WindowPropertys& windowProps);
    ~Application();

    void renderInit();

    void mainLoop();
    void update(float dT);
    void render();
    
private:
    Window m_Window;
    WindowPropertys m_WindowProps;
    AppState m_AppState;
    EventStack m_Stack;

    uint m_ProgramID;
    std::vector<Metaheuristic*> m_Metaheuristics;


    ChronoClock<std::chrono::milliseconds> m_Clock;
};
