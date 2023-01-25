#pragma once

#include "pch.h"
#include "Debug.h"

#define qwerty 0
#define colemak 1

// structures

struct KeyEvent {
    typedef void (*keyDown)();
    typedef void (*keyUp)();

    std::string key;
    std::vector<keyDown> keyDownFunctions;
    std::vector<keyUp> keyUpFunctions;
};

struct Cursor {
    int x;
    int y;
    int relX;
    int relY;
};

struct MouseWheel {
    int x;
    int y;
    uint32_t direction;
};

struct MouseButton {
    bool leftClickDown;
    bool rightClickDown;
    bool leftClickUp;
    bool rightClickUp;
    int x;
    int y;
};

struct Mouse {
    Cursor cursor;
    MouseWheel wheel;
    MouseButton button;
};

struct AppState {
    bool running;
    Mouse mouse;
};

// used to allow for easy swaping of controls in say, menu vs game
struct EventStack {
    typedef void (*eventFunction)();
    std::unordered_map<std::string, KeyEvent> keyEvents;
};

// functions

void pollEvents(AppState& state, EventStack& stack);
void eventSystemInit(AppState& state, uint layout);

std::string getKeyFromKeycode(uint keycode);

void subscribeToKeyEvent(EventStack& stack, std::string key, void (*functionDown)(), void (*functionUp)() = nullptr);

