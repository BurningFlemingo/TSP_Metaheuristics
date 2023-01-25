#pragma once

#include "pch.h"

#define uint unsigned int

struct WindowPropertys {
    std::string title;
    uint windowWidth;
    uint windowHeight;
    uint flags;
};

struct Window {
    ~Window();

    SDL_Window* window;
    SDL_GLContext context;
};

void initSDL();
void initOpenGL();
Window& createWindow(const WindowPropertys& windowProps);
void presentWindow(const Window& window);
