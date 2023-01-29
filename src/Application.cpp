#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Shapes.h"
#include "GravityParticleSystem.h"
#include "LorenzAttractor.h"

float rotation = 0;

void rotateRight() {
    rotation += 3;
}
void rotateLeft() {
    rotation -= 3;
}

Application::Application(const WindowPropertys& windowProps) : m_WindowProps(windowProps) {
    m_Window = createWindow(windowProps);
    eventSystemInit(m_AppState, colemak);

    srand(time(0));

    m_AppState.running = true;
}

Application::~Application() {
    log("Application destructor called", INFO);
    SDL_Quit();
}

void Application::renderInit() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_ProgramID = setupShaders("../res/Shaders/VertexShader.glsl", "../res/Shaders/FragmentShader.glsl"); 
    glUseProgram(m_ProgramID);

    setProjectionMatrix(0, m_WindowProps.windowWidth, m_WindowProps.windowHeight, 0, -1000, 1000);

    subscribeToKeyEvent(m_Stack, "s", &rotateRight);    
    subscribeToKeyEvent(m_Stack, "a", &rotateLeft);    

    setUniform4m(m_ProgramID, "projection", getProjectionMatrix());

    int halfW = m_WindowProps.windowWidth / 2;
    int halfH = m_WindowProps.windowHeight / 2;
    int wW = m_WindowProps.windowWidth;
    int wH = m_WindowProps.windowHeight;

    LorenzAttractor* lR = new LorenzAttractor({100, 2}, 10, 28, (8.0/3.0));  // ammount, size
    lR->spawnRandomParticles({1, 2}, {1, 2}, {1, 2});
    m_Metaheuristics.emplace_back(lR);
    // GPS* gps = new GPS({100, 10}, &rotation);
    // gps->spawnRandomParticles({0, 250, 0, 250}, 5, 10);
    // m_Metaheuristics.emplace_back(gps);
}

void Application::mainLoop() {
    while (m_AppState.running) {  // runs at ~19 milliseconds per tick 
        pollEvents(m_AppState, m_Stack);  // 0 - 1 milliseconds
        m_Clock.startTimer();
        update(0.01);
        render();  // ~17 ms per tick
        print(m_Clock.endTimer());
    }
}

void Application::update(float dT) {
    for (auto& metaheuristic : m_Metaheuristics) {
        metaheuristic->update(dT);
    }
}

void Application::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (auto& metaheuristic : m_Metaheuristics) {
        metaheuristic->draw();
    }
    SDL_GL_SwapWindow(m_Window.window);
}
