#include "pch.h"
#include "Application.h"

const uint windowWidth = 1920 / 2;
const uint windowHeight = 1080 / 2;

int main(int argc, char* argv[]) {
    Application app({ "PROJECT", 1365, 770, 0 });
    app.run();

    SDL_Quit();
    return 0;
}
