#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Shapes.h"
#include "ComputeShaders/SlimeMold.h"

float rotation = 0;

void rotateRight() {
    rotation += 3;
}
void rotateLeft() {
    rotation -= 3;
}

Application::Application(const WindowPropertys& windowProps)
: m_WindowProps(windowProps), m_MaxDeltaTime(3.0), m_MinDeltaTime(0.0), m_DeltaTime(0.0), m_LastTime(0) {
    m_Window = createWindow(windowProps);
    eventSystemInit(m_AppState, colemak);

    srand(time(0));

    ComputeSettings settings;
    settings.windowDimensions = glm::vec2(m_WindowProps.windowWidth, m_WindowProps.windowHeight);
    settings.scale = 1.0f;
    settings.workGoupSize = 16;
    settings.decay = 0.2;
    settings.diffusionRate = 0.3;
    Shaders shaders;
    shaders.vertexShader = "../res/Shaders/Window.vert";
    shaders.fragmentShader = "../res/Shaders/Window.frag";
    SlimeMoldSettings moldSettings;
    moldSettings.count = 100000;
    m_ComputeShaders.emplace_back(std::make_unique<SlimeMold>(settings, shaders, moldSettings));

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

    setProjectionMatrix(0, m_WindowProps.windowWidth, m_WindowProps.windowHeight, 0, 0, 1);

    subscribeToKeyEvent(m_Stack, "s", &rotateRight);    
    subscribeToKeyEvent(m_Stack, "a", &rotateLeft);    

    // setUniform4m(m_ProgramID, "projection", getProjectionMatrix());

    int halfW = m_WindowProps.windowWidth / 2;
    int halfH = m_WindowProps.windowHeight / 2;
    int wW = m_WindowProps.windowWidth;
    int wH = m_WindowProps.windowHeight;

    for (const auto& shader : m_ComputeShaders) {
        shader->setup();
    }
}

void Application::deltaUpdate() {
    uint32_t currentTime = m_Clock.getTime();
    uint32_t difference = currentTime - m_LastTime;
    m_DeltaTime = (difference % ((uint)(m_MaxDeltaTime * 1000000) - (uint)(m_MinDeltaTime * 1000000) + 1) + 1) / 1000000.0;
    m_LastTime = currentTime;
}

void Application::mainLoop() {
    m_LastTime = m_Clock.getTime();

    while (m_AppState.running) {
        deltaUpdate();
        pollEvents(m_AppState, m_Stack);
        render();
    }
}

void Application::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& shader : m_ComputeShaders) {
        shader->run(m_DeltaTime);
    }

    SDL_GL_SwapWindow(m_Window.window);
}
