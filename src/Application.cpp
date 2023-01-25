#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Shapes.h"
#include "Particles.h"

float rotation = 0;
int tick = 0;

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

    setProjectionMatrix(0, m_WindowProps.windowWidth, m_WindowProps.windowHeight, 0, -1, 1);

    subscribeToKeyEvent(m_Stack, "s", &rotateRight);    
    subscribeToKeyEvent(m_Stack, "a", &rotateLeft);    

    setUniform4m(m_ProgramID, "projection", getProjectionMatrix());

    int halfW = m_WindowProps.windowWidth / 2;
    int halfH = m_WindowProps.windowHeight / 2;
    int wW = m_WindowProps.windowWidth;
    int wH = m_WindowProps.windowHeight;

    ParticleSystem* particleSystem = new ParticleSystem(m_AppState);
    particleSystem->createRandomParticles(glm::vec2(wW, wH), 10000, 1, 5);
    m_Systems.emplace_back(particleSystem);
}

void Application::mainLoop() {
    while (m_AppState.running) {  // runs at ~19 milliseconds per tick 
        pollEvents(m_AppState, m_Stack);  // 0 - 1 milliseconds
        update(0);
        measure(render());  // ~17 ms per tick
    }
}

void Application::update(float dT) {
    for (auto& system : m_Systems) {
        system->update(dT);
    }
}

void Application::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (auto& system : m_Systems) {
        system->draw();
    }
    SDL_GL_SwapWindow(m_Window.window);
}