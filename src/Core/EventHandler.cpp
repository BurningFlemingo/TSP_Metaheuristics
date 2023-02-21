#include "pch.h"
#include "Debug.h"
#include "EventHandler.h"

std::unordered_map<uint, std::string> keymap;
std::unordered_map<std::string, bool> keyDown;

void fillKeyCodeArrays(uint layout) {
    if (layout > 2 || layout < 0) {
        log("Unregistered layout", ERROR);
    }
    
    switch (layout) {
        case 0: {
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
        case 1: {
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
