#include "pch.h"
#include "Application.h"

const uint windowWidth = 1920 / 2;
const uint windowHeight = 1080 / 2;

int main(int argc, char* argv[]) {
    Application app({ "title", windowWidth, windowHeight, 0 });
    app.renderInit();
    app.mainLoop();

    SDL_Quit();
    return 0;
}
