#include "pch.h"

#define DEBUG

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define measure(x) int MEASURED_TICKS = SDL_GetTicks(); \
                   x; \
                   print(SDL_GetTicks() - MEASURED_TICKS) \

#ifdef DEBUG
    #define NOTHING 0
    #define MESSAGE 1
    #define INFO 2
    #define WARNING 3
    #define ERROR 4
    #define SEVERE_ERROR 5

    #define glCheckError(x) x; \
        if (glGetError() != GL_NO_ERROR) { \
            std::cout << "Location: " << __FILENAME__ << ":" << __LINE__ << "\n --------------- " << std::endl; \
            std::exit(0); \
        } \

    #define logf(x, y) std::cout << __FILENAME__ << ":" << __LINE__ << ": " << __FUNCTION__ << ": "<< x << " : " << y << std::endl; std::terminate 
    #define print(x) std::cout << x << std::endl;
    #define print2(x, y) std::cout << x << " " << y << std::endl;

    #define log(msg, severity) switch(severity) { \
        case 0: { \
            break; \
        } \
        case 1: { \
            std::cout << "|MESSAGE| " << __FILENAME__ << ":" << __LINE__ << ": " << __FUNCTION__ << ": "<< msg << std::endl; \
            break; \
        } \
        case 2: { \
            std::cout << "|INFO| " << __FILENAME__ << ":" << __LINE__ << ": " << __FUNCTION__ << ": "<< msg << std::endl; \
            break; \
        } \
        case 3: { \
            std::cout << "|WARNING| " << __FILENAME__ << ":" << __LINE__ << ": " << __FUNCTION__ << ": "<< msg << std::endl; \
            break; \
        } \
        case 4: { \
            std::cout << "|ERROR| " << __FILENAME__ << ":" << __LINE__ << ": " << __FUNCTION__ << ": "<< msg << std::endl; \
            std::terminate(); \
            break; \
        } \
        case 5: { \
            std::cout << "|SEVERE ERROR| " << __FILENAME__ << ":" << __LINE__ << ": " << __FUNCTION__ << ": "<< msg << std::endl; \
            std::terminate(); \
            break; \
        } \
    } \

#endif
#ifdef RELEASE
    #define NOTHING 0
    #define MESSAGE 0
    #define INFO 0
    #define WARNING 0
    #define ERROR 4
    #define SEVERE_ERROR 5   

    #define checkError(x) x

    #define logf(x, y) 
    #define print(x)
    #define print2(x, y)
    #define log(msg, severity)
#endif


