#include "pch.h"
#include "Debug.h"
#include "EventHandler.h"

std::unordered_map<uint, std::string> keymap;
std::unordered_map<std::string, bool> keyDown;

std::unordered_map<uint32_t, std::string> mouseMap;

void pollEvents(AppState& state, EventStack& stack) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                std::string key = getKeyFromKeycode(event.key.keysym.scancode);

                if (key == "esc") {
                    state.running = false;
                }
                
                auto iterator = stack.keyEvents.find(key);
                if (iterator != stack.keyEvents.end()) {
                    keyDown.at(key) = true;
                    break;
                }
                break;               
            }
            case SDL_KEYUP: {
                std::string key = getKeyFromKeycode(event.key.keysym.scancode);

                auto iterator = stack.keyEvents.find(key);
                if (iterator != stack.keyEvents.end()) {
                    if (iterator->second.keyUpFunctions.size() != 0) {
                        for (auto function : iterator->second.keyUpFunctions) {
                            function();
                        }
                    }
                    keyDown.at(key) = false;
                    break;
                }
                break;
            }
            case SDL_MOUSEMOTION: {
                state.mouse.cursor.x = event.motion.x;
                state.mouse.cursor.y = event.motion.y;
                state.mouse.cursor.relX = event.motion.xrel;
                state.mouse.cursor.relY = event.motion.yrel;
                break;
            }
            case SDL_MOUSEWHEEL: {
                state.mouse.wheel.x = event.wheel.x;
                state.mouse.wheel.y = event.wheel.y;
                state.mouse.wheel.direction = event.wheel.direction;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    state.mouse.button.leftClickUp = false;
                    state.mouse.button.leftClickDown = true;
                    break;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    state.mouse.button.rightClickUp = false;
                    state.mouse.button.rightClickDown = true;
                    break;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    state.mouse.button.leftClickUp = true;
                    state.mouse.button.leftClickDown = false;
                    break;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    state.mouse.button.rightClickUp = true;
                    state.mouse.button.rightClickDown = false;
                    break;
                }
                break;
            }
            case SDL_QUIT:
                log("SDL_Quit event fired", INFO);
                SDL_Quit();
                break;
            default:
                break;
        }
    }

    for (auto event : stack.keyEvents) {
        if (keyDown.at(event.first)) {
            for (auto function : event.second.keyDownFunctions) {
                function();
            }
        }
    }
}

void eventSystemInit(AppState& state, uint layout) {
    if (layout > 2 || layout < 0) {
        log("Unregistered layout", ERROR);
    }
    
    switch (layout) {
        case qwerty: {
            // alphabet
            for (int i = 4; i < 30; i++) {
                char key = 'a' + i - 4;
                std::string keyString = "";
                keyString.push_back(key);
                keymap.insert({ i, keyString });
            }

            // special keys
            keymap.insert({ 41, "esc" });
            keymap.insert({ 43, "tab" });
            keymap.insert({ 44, "space" });
            keymap.insert({ 42, "enter"});
            keymap.insert({ 40, "backspace" });

            // modifiers
            keymap.insert({ 57, "capslock" });
            keymap.insert({ 225, "shift" });
            keymap.insert({ 224, "ctrl" });
            keymap.insert({ 226, "alt" });

            // numbers
            for (int i = 30; i < 39; i++) {
                char key = '1' + i - 30;
                std::string keyString = "";
                keyString.push_back(key);
                keymap.insert({ i, keyString });
            }
            keymap.insert({ 39, std::string("0") });

            break;
        }
        case colemak: {
            // alphabet
            keymap.insert({ 4, "a" });
            keymap.insert({ 5, "b" });
            keymap.insert({ 6, "c" });
            keymap.insert({ 10, "d" });
            keymap.insert({ 14, "e" });
            keymap.insert({ 8, "f" });
            keymap.insert({ 23, "g" });
            keymap.insert({ 11, "h" });
            keymap.insert({ 15, "i" });
            keymap.insert({ 28, "j" });
            keymap.insert({ 17, "k" });
            keymap.insert({ 24, "l" });
            keymap.insert({ 16, "m" });
            keymap.insert({ 13, "n" });
            keymap.insert({ 51, "o" });
            keymap.insert({ 21, "p" });
            keymap.insert({ 20, "q" });
            keymap.insert({ 22, "r" });
            keymap.insert({ 7, "s" });
            keymap.insert({ 9, "t" });
            keymap.insert({ 12, "u" });
            keymap.insert({ 25, "v" });
            keymap.insert({ 26, "w" });
            keymap.insert({ 27, "x" });
            keymap.insert({ 18, "y" });
            keymap.insert({ 29, "z" });

            // special keys
            keymap.insert({ 41, "esc" });
            keymap.insert({ 43, "tab" });
            keymap.insert({ 44, "space" });
            keymap.insert({ 40, "backspace" });
            keymap.insert({ 42, "enter"});

            // modifiers
            keymap.insert({ 57, "capslock" });
            keymap.insert({ 225, "shift" });
            keymap.insert({ 224, "ctrl" });
            keymap.insert({ 226, "alt" });

            // numbers
            for (int i = 30; i < 39; i++) {
                char key = '1' + i - 30;
                std::string keyString = "";
                keyString.push_back(key);
                keymap.insert({ i, keyString });
            }
            keymap.insert({ 39, std::string("0") });
            break;
        }
    }

    log("Event System initialized", INFO);
}

std::string getKeyFromKeycode(uint keycode) {
    auto iterator = keymap.find(keycode);
    if (iterator != keymap.end()) {
        return iterator->second;
    }
    return "INVALID KEY";
}

void addKeyEvent(EventStack& stack, const KeyEvent& event) {
    std::string key = event.key;

    stack.keyEvents.insert({ key, event });
    keyDown.insert({ key, false });
}

void subscribeToKeyEvent(EventStack& stack, std::string key, void (*functionDown)(), void (*functionUp)()) {
    if (stack.keyEvents.find(key) != stack.keyEvents.end()) {
        if (functionDown != nullptr) {
            stack.keyEvents.at(key).keyDownFunctions.emplace_back(functionDown);
        }
        if (functionUp != nullptr) {
            stack.keyEvents.at(key).keyUpFunctions.emplace_back(functionUp);
        }
    } else {
        KeyEvent event;
        event.key = key;
        if (functionDown != nullptr) {
            event.keyDownFunctions.emplace_back(functionDown); 
        }
        if (functionUp != nullptr) {
            event.keyUpFunctions.emplace_back(functionUp);
        }
            addKeyEvent(stack, event);
    }
}
