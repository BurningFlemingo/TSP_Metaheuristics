#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Shapes.h"
#include "ComputeShaders/SlimeMold.h"
#include "ComputeShaders/AntColony.h"

Application::Application(const WindowPropertys& windowProps)
: m_WindowProps(windowProps), m_MaxDeltaTime(3.0), m_MinDeltaTime(0.0), m_DeltaTime(0.0), m_LastTime(0), m_SimulationsCompleted(false),
  m_SimulationsRun(0), paused(false) {
    m_Window = createWindow(windowProps);
    printOpenglInfo();
    fillKeyCodeArrays(1);
    
    seedRandNum(m_Clock.getTime());

    initRenderer();

    m_Running = true;
}

Application::~Application() {
    SDL_Quit();

    if (m_SimulationsCompleted) {
        graphData();
    }
}

void Application::initTSP(uint numberOfCitys) {
    TSPSettings setTsp;
    setTsp.numberOfCitys = numberOfCitys;
    setTsp.spawnArea = glm::vec2(m_WindowProps.windowWidth, m_WindowProps.windowHeight);
    setTsp.cityResolution = 20;
    setTsp.cityRadius = 6;
    setTsp.lineWidth = 3;
    setTsp.cityColor = { 255, 255, 255, 255 };
    m_TSP = std::make_shared<TSP>(setTsp);

    m_TSP->generateCityTexture(2);
}

void Application::spawnSMO(const Parameters& parameters) {
    ComputeSettings settings;
    settings.windowDimensions = glm::vec2(m_WindowProps.windowWidth, m_WindowProps.windowHeight);
    settings.scale = 1.0f;
    settings.workGoupSize = 64;
    settings.decay = parameters.decay;
    settings.diffusionRate = parameters.diffusion;
    Shaders shaders;
    shaders.vertexShader = "../res/Shaders/Window.vert";
    shaders.fragmentShader = "../res/Shaders/Window.frag";
    SlimeMoldSettings moldSettings;
    moldSettings.count = parameters.numberOfParticles;
    moldSettings.color = parameters.color;
    moldSettings.detectionThreshold = parameters.detectionThreshold;
    moldSettings.trailReleseThreshold = parameters.trailReleseThreshold;
    moldSettings.turnThreshold = parameters.turnThreshold;
    moldSettings.foodValue = parameters.foodValue;
    moldSettings.forwardRayDistance = parameters.forwardRayDistance;
    moldSettings.sideRayDistances = parameters.sideRayDistances;
    moldSettings.title = parameters.title;
    m_ComputeShader = std::make_unique<SlimeMold>(settings, shaders, m_TSP, moldSettings);
    m_ComputeShader->setup();
}

void Application::spawnACO(uint count, float decay, float diffusionRate) {
    ComputeSettings settings;
    settings.windowDimensions = glm::vec2(m_WindowProps.windowWidth, m_WindowProps.windowHeight);
    settings.scale = 1.0f;
    settings.workGoupSize = 64;
    settings.decay = decay;
    settings.diffusionRate = diffusionRate;
    Shaders shaders;
    shaders.vertexShader = "../res/Shaders/Window.vert";
    shaders.fragmentShader = "../res/Shaders/Window.frag";
    AntSettings antSettings;
    antSettings.count = count;
    m_ComputeShader = std::make_unique<AntColony>(settings, shaders, m_TSP, antSettings);
    m_ComputeShader->setup();
}

void Application::initRenderer() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setProjectionMatrix(0, m_WindowProps.windowWidth, m_WindowProps.windowHeight, 0, 0, 1);
}

bool Application::runMetaheuristic(uint ticks) {
    for (int j = 0; j < ticks; j++) {
        if (!m_Running) {
            return true;
        }
        while (paused) {
            pollEvents();
        }
        pollEvents();
        update();
        render();
    }

    collectData();
    m_ComputeShader.reset();

    return false;
}

bool Application::runSimulation(const Parameters& parameter) {
        Parameters parameters;
        parameters.numberOfSimulations = 100000;
        parameters.ticksPerSimulation = 200;
        parameters.diffusion = 0.4;
        parameters.decay = 0.4;
        parameters.numberOfCitys = 8;
        parameters.forwardRayDistance = 10;
        parameters.sideRayDistances = 20;
        parameters.foodValue = 10000;
        parameters.trailReleseThreshold = 0.001;
        parameters.detectionThreshold = 0.1;
        parameters.numberOfParticles = 200000;
        parameters.turnThreshold = 0.001;
        parameters.color = { 0.5, 0.0, 0.0 };
        parameters.title = "1";

    for (int i = 0; i < parameters.numberOfSimulations; i++) {
        bool returned = false;
        print("SIM: " + std::to_string(i + 1));
        initTSP(parameters.numberOfCitys);

        for (int a = 1; a < 6; a++) {
            parameters.decay = 0.2 * a;
            for (int b = 1; b < 6; b++) {
                parameters.diffusion = 0.2 * b;
                for (int c = 0; c < 3; c++) {
                    parameters.foodValue = std::pow(100, c);
                    for (int d = 1; d < 4; d++) {
                        parameters.turnThreshold = 0.001 * pow(10, d);
                        for (int e = 1; e < 5; e++) {
                            parameters.forwardRayDistance = e * 10;
                            for (int f = 1; f < 5; f++) {
                                parameters.sideRayDistances = f * 10;
                                parameters.color = {randNum(0, 255) / 255.0 , randNum(0, 255) / 255.0,  randNum(0, 255) / 255.0};

                                print("number of particles: " + std::to_string(parameters.numberOfParticles));
                                print("");
                                print("decay: " + std::to_string((int)(parameters.decay)) + "%");
                                print("diffusion: " + std::to_string((int)(parameters.diffusion * 100)) + "%");
                                print("food value: " + std::to_string((int)parameters.foodValue));
                                print("turn threshold: " + std::to_string((int)(parameters.turnThreshold * 1000)) + "%");
                                print("forwardRayDistance: " + std::to_string((int)parameters.forwardRayDistance));
                                print("sideRayDistances: " + std::to_string((int)parameters.sideRayDistances));

                                spawnSMO(parameters);
                                returned = runMetaheuristic(parameters.ticksPerSimulation);
                                if (returned == true) {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (returned == true) {
            return false;
        }

        setData();
        printData();
        m_TSP.reset();
    }

    m_SimulationsRun++;
    return true;
}

void Application::deltaUpdate() {
    uint32_t currentTime = m_Clock.getTime();
    uint32_t difference = currentTime - m_LastTime;
    m_DeltaTime = (difference % ((uint)(m_MaxDeltaTime * 1000000) - (uint)(m_MinDeltaTime * 1000000) + 1) + 1) / 1000000.0;
    m_LastTime = currentTime;
}

void Application::run() {
    m_LastTime = m_Clock.getTime();

    Parameters parameters;
    parameters.ticksPerSimulation = 200;
    parameters.numberOfSimulations = 20;

    while (m_Running) {
        bool completed = runSimulation(parameters);

        if (completed) {
            m_Running = false;
            m_SimulationsCompleted = true;
        }
    }
}

void Application::pollEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                std::string key = getKeyFromKeycode(event.key.keysym.scancode);

                if (key == "esc") {
                    m_Running = false;
                }
                if (key == "space") {
                    paused = true;
                }
                break;               
            }
            case SDL_KEYUP: {
                std::string key = getKeyFromKeycode(event.key.keysym.scancode);
                if (key == "space") {
                    paused = false;
                }
                break;
            }
            case SDL_MOUSEMOTION: {
                break;
            }
            case SDL_MOUSEWHEEL: {
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                break;
            }
            case SDL_QUIT:
                SDL_Quit();
                break;
            default:
                break;
        }
    }
}

void Application::update() {
    m_ComputeShader->update(0.016777);

}

void Application::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ComputeShader->render();

    m_TSP->renderCitys();

    SDL_GL_SwapWindow(m_Window.window);
}

void Application::collectData() {
    Data data = m_ComputeShader->exportData();

    if (m_TrialData.size() == 0) {
        Data tspData;

        tspData.title = "TSP best possible path (control)";
        tspData.sizeofPath = m_TSP->getSizeOfBestPath();
        m_TrialData.emplace_back(tspData);
    }

    m_TrialData.emplace_back(data);
}


void Application::setData() {
    m_CollectedData.emplace_back(m_TrialData);
    ws = wb.active_sheet();

    for (int i = 0; i < m_CollectedData.size(); i++) {
        for (int j = 0; j < m_TrialData.size(); j++) {
            ws.cell(i + 2, j + 1).value(m_CollectedData[i][j].sizeofPath);
        }
    }

    wb.save("Data.xlsx");

    m_TrialData.clear();
}

void Application::printData() {
    print("---------------");
    print("---------------");
    print("Data dump for sim");
    print("---------------");

    for (int i = 0; i < m_CollectedData[0].size(); i++) {
        for (int j = 0; j < m_CollectedData.size(); j++) {
            std::cout << i << ": " << m_CollectedData[j][i].sizeofPath << std::endl;
        }
    }
}

void Application::graphData() {
    if (m_CollectedData[0].size() == 0) {
        for (int i = 0; i < m_CollectedData[0].size(); i++) {
            ws.cell(i, 1).value(m_CollectedData[0][i].title);
        }
    }

    wb.save("Data.xlsl");

    std::vector<double> xAxis = matplot::linspace(0, m_CollectedData.size());
    matplot::plot(xAxis);
    std::vector<double> xTicks;

    std::vector<std::vector<double>> data;

    for (int i = 0; i < m_CollectedData[0].size(); i++) {
        data.emplace_back(std::vector<double>());
    }
    for (int i = 0; i < m_CollectedData.size(); i++) {
        for (int j = 0; j < m_CollectedData[0].size(); j++) {
            data[j].emplace_back(m_CollectedData[i][j].sizeofPath);
        }
    }

    for (int i = 0; i < 101;) {
        xTicks.emplace_back(i);
        i += 2;
    }

    matplot::plot(data[0]);
    matplot::hold(true);
    for (int i = 1; i < m_CollectedData[0].size(); i++) {
        matplot::plot(data[i]);
    }
    matplot::hold(false);

    matplot::title("Fig. 2: Best path found per trial with slime molds having different diffusions");
    matplot::xlabel("Trial");
    matplot::ylabel("Distance (px)");
    matplot::xticks(xTicks);

    std::vector<std::string> titles;
    for (int i = 0; i < m_CollectedData[0].size(); i++) {
        titles.emplace_back(m_CollectedData[0][i].title);
    }

    matplot::legend(titles);
    matplot::show();
}
