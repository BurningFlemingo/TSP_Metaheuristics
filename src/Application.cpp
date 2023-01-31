#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Shapes.h"
#include "Compute.h"

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

    ComputeSettings settings;
    settings.windowDimensions = glm::vec2(m_WindowProps.windowWidth, m_WindowProps.windowHeight);
    settings.scale = 1.0f;
    settings.workGoupSize = 128;
    m_Template = std::make_unique<Compute>(settings);

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

    m_Template->glSetup();
}

void Application::mainLoop() {
    while (m_AppState.running) {
        pollEvents(m_AppState, m_Stack);
        m_Clock.startTimer();
        // update(0.01);
        render();
        print(m_Clock.endTimer());
    }
}

void Application::update(float dT) {
}

void Application::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Template->draw();

    SDL_GL_SwapWindow(m_Window.window);
}
